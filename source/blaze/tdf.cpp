
// Include
#include "tdf.h"
#include "types.h"
#include "utils/logger.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <iostream>

#ifdef _MSC_VER
#	define BSWAP16 _byteswap_ushort
#	define BSWAP32 _byteswap_ulong
#	define BSWAP64 _byteswap_uint64
#else
#	define BSWAP16 __builtin_bswap16
#	define BSWAP32 __builtin_bswap32
#	define BSWAP64 __builtin_bswap64
#endif

// Blaze
namespace Blaze {
	void Log(const rapidjson::Document& document) {
		rapidjson::StringBuffer buffer;
		buffer.Clear();

		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		std::string documentString = buffer.GetString();
		logger::log(documentString); 
	}

	// TDF
	namespace TDF {
		uint32_t CompressLabel(const std::string& label) {
			uint32_t ret = 0;
			for (size_t i = 0; i < std::min<size_t>(label.size(), 4); ++i) {
				ret |= (0x20 | (label[i] & 0x1F)) << ((3 - i) * 6);
			}
			return BSWAP32(ret) >> 8;
		}

		std::string DecompressLabel(uint32_t label) {
			label = BSWAP32(label) >> 8;

			std::string ret;
			for (uint32_t i = 0; i < 4; ++i) {
				uint32_t val = (label >> ((3 - i) * 6)) & 0x3F;
				if (val > 0) {
					ret.push_back(static_cast<char>(0x40 | (val & 0x1F)));
				}
			}

			return ret;
		}

		Header ReadHeader(DataBuffer& buffer) {
			Header header;

			uint32_t value = buffer.read<uint32_t>();
			header.type = static_cast<Blaze::TDF::Type>(value >> 24);
			header.label = DecompressLabel(value & 0x00FFFFFF);

			return header;
		}

		void WriteHeader(DataBuffer& buffer, const Header& header) {
			WriteHeader(buffer, header.label, header.type);
		}

		void WriteHeader(DataBuffer& buffer, const std::string& label, Type type) {
			buffer.write<uint32_t>(CompressLabel(label) | (static_cast<uint32_t>(type) << 24));
		}

		void WriteUnionHeader(DataBuffer& buffer, const std::string& label, NetworkAddressMember activeMember) {
			WriteHeader(buffer, label, Type::Union);
			buffer.write<uint8_t>(static_cast<uint8_t>(activeMember));
		}

		uint64_t read_integer(DataBuffer& buffer) {
			return buffer.decode_tdf_integer();
		}

		void WriteInteger(DataBuffer& buffer, const std::string& label, uint64_t value) {
			WriteHeader(buffer, { label, Type::Integer });
			buffer.encode_tdf_integer(value);
		}

		std::string read_string(DataBuffer& buffer) {
			std::string str;

			uint64_t length = buffer.decode_tdf_integer() - 1;
			if (length > 0) {
				str.resize(length);
				buffer.read<char>(&str[0], length);
			}

			buffer.skip<char>();
			return str;
		}

		void WriteString(DataBuffer& buffer, const std::string& label, const std::string& value) {
			uint64_t length = static_cast<uint64_t>(value.length());

			WriteHeader(buffer, { label, Type::String });
			buffer.encode_tdf_integer(length + 1);

			buffer.write<char>(&value[0], length);
			buffer.write<char>(0);
		}

		// Parser
		Parser::Parser() : mDocument(rapidjson::kObjectType) {}

		void Parser::Read(DataBuffer& buffer) {
			rapidjson::Document(rapidjson::kObjectType).Swap(mDocument);
			// mDocument.SetObject();
			while (!buffer.eof()) {
				ReadValue(buffer, mDocument);
			}
		}

		void Parser::ReadValue(DataBuffer& buffer, rapidjson::Value& parent) {
			const auto AddObject = [this, &parent](const Header& header, rapidjson::Value& value) {
				auto& allocator = mDocument.GetAllocator();
				if (parent.IsObject()) {
					rapidjson::Value key(header.label, allocator);

					auto member = parent.FindMember(key);
					if (member != parent.MemberEnd()) {
						member->value = value;
					} else {
						parent.AddMember(key, value, allocator);
					}
				} else if (parent.IsArray()) {
					parent.PushBack(value, allocator);
				}
			};

			auto position = buffer.position();

			Header header = ReadHeader(buffer);
			Header previousHeader = std::move(mHeader);

			mHeader = header;
			if (buffer.eof()) {
				return;
			}

			// std::cout << "Label: " << mHeader.label << ", Type: " << static_cast<int>(mHeader.type) << std::endl;
			switch (mHeader.type) {
				case Type::Integer: {
					rapidjson::Value value = ReadInteger(buffer);
					AddObject(header, value);
					break;
				}

				case Type::String: {
					rapidjson::Value value = ReadString(buffer);
					AddObject(header, value);
					break;
				}

				case Type::Binary: {
					rapidjson::Value value = ReadBlob(buffer);
					AddObject(header, value);
					break;
				}

				case Type::Struct: {
					rapidjson::Value value = ReadStruct(buffer);
					AddObject(header, value);
					break;
				}

				case Type::Union: {
					rapidjson::Value value = ReadUnion(buffer);
					AddObject(header, value);
					break;
				}

				case Type::List: {
					rapidjson::Value value = ReadList(buffer);
					AddObject(header, value);
					break;
				}

				case Type::Map: {
					rapidjson::Value value = ReadMap(buffer);
					AddObject(header, value);
					break;
				}

				case Type::IntegerList: {
					rapidjson::Value value = ReadIntegerList(buffer);
					AddObject(header, value);
					break;
				}

				case Type::ObjectType: {
					rapidjson::Value value = ReadObjectType(buffer);
					AddObject(header, value);
					break;
				}

				case Type::ObjectId: {
					rapidjson::Value value = ReadObjectId(buffer);
					AddObject(header, value);
					break;
				}

				default: {
					std::cout << "Unknown header(" << position << ", " << header.label << "): " << static_cast<int>(header.type)
						<< ", Previous type(" << previousHeader.label << "): " << static_cast<int>(previousHeader.type) << std::endl;
					__debugbreak();
					break;
				}
			}
		}

		rapidjson::Value Parser::ReadInteger(DataBuffer& buffer) {
			return rapidjson::Value(read_integer(buffer));
		}

		rapidjson::Value Parser::ReadString(DataBuffer& buffer) {
			return rapidjson::Value(read_string(buffer), mDocument.GetAllocator());
		}

		rapidjson::Value Parser::ReadBlob(DataBuffer& buffer) {
			std::string str;

			uint64_t length = read_integer(buffer);
			if (length > 0) {
				str.resize(length);
				buffer.read<char>(&str[0], length);
			}

			return rapidjson::Value(str, mDocument.GetAllocator());
		}

		rapidjson::Value Parser::ReadStruct(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::Struct)), allocator);

			while (buffer.peek<uint8_t>() != 0x00) {
				ReadValue(buffer, value);
			}

			buffer.skip<uint8_t>();
			return value;
		}

		rapidjson::Value Parser::ReadUnion(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::Union)), allocator);

			uint8_t addressMember = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_AddressMember", allocator), rapidjson::Value(addressMember), allocator);

			if (static_cast<NetworkAddressMember>(addressMember) != NetworkAddressMember::Unset) {
				ReadValue(buffer, value);
			}

			return value;
		}

		rapidjson::Value Parser::ReadList(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::List)), allocator);

			uint8_t listType = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_ListType", allocator), rapidjson::Value(listType), allocator);

			uint8_t listSize = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_ListSize", allocator), rapidjson::Value(listSize), allocator);

			rapidjson::Value listContent(rapidjson::kArrayType);

			Type type = static_cast<Type>(listType);
			if (buffer.peek<uint8_t>() == 0x02 && type == Type::Struct) {
				value.AddMember(rapidjson::Value("_Stub", allocator), rapidjson::Value(true), allocator);

				buffer.skip<uint8_t>();
				while (buffer.peek<uint8_t>() != 0x00) {
					ReadValue(buffer, listContent);
				}
				buffer.skip<uint8_t>();
			} else {
				value.AddMember(rapidjson::Value("_Stub", allocator), rapidjson::Value(false), allocator);
				for (uint8_t i = 0; i < listSize; i++) {
					switch (type) {
						case Type::Integer: {
							rapidjson::Value contentValue = ReadInteger(buffer);
							listContent.PushBack(contentValue, allocator);
							break;
						}

						case Type::String: {
							rapidjson::Value contentValue = ReadString(buffer);
							listContent.PushBack(contentValue, allocator);
							break;
						}

						case Type::Struct: {
							rapidjson::Value contentValue = ReadStruct(buffer);
							listContent.PushBack(contentValue, allocator);
							break;
						}

						default:
							break;
					}
				}
			}

			value.AddMember(rapidjson::Value("_Content", allocator), listContent, allocator);
			return value;
		}

		rapidjson::Value Parser::ReadMap(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			const auto GetMapItem = [&](Type type) {
				rapidjson::Value value;
				switch (type) {
					case Type::Integer:
						value = ReadInteger(buffer);
						break;

					case Type::String:
						value = ReadString(buffer);
						break;

					case Type::Struct:
						value = ReadStruct(buffer);
						break;

					default:
						break;
				}
				return value;
			};

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::Map)), allocator);

			uint8_t keyType = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_KeyType", allocator), rapidjson::Value(keyType), allocator);

			uint8_t valueType = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_ValueType", allocator), rapidjson::Value(valueType), allocator);

			uint8_t mapSize = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_MapSize", allocator), rapidjson::Value(mapSize), allocator);

			rapidjson::Value mapContent(rapidjson::kObjectType);
			for (uint8_t i = 0; i < mapSize; ++i) {
				auto key = GetMapItem(static_cast<Type>(keyType));
				auto value = GetMapItem(static_cast<Type>(valueType));
				if (static_cast<Type>(keyType) == Type::Integer) {
					key = rapidjson::Value(std::to_string(key.GetUint64()), allocator);
				}
				mapContent.AddMember(key, value, allocator);
			}

			value.AddMember(rapidjson::Value("_Content", allocator), mapContent, allocator);
			return value;
		}

		rapidjson::Value Parser::ReadIntegerList(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::IntegerList)), allocator);

			uint8_t listSize = buffer.read<uint8_t>();
			value.AddMember(rapidjson::Value("_ListSize", allocator), rapidjson::Value(listSize), allocator);

			rapidjson::Value listContent(rapidjson::kArrayType);
			for (uint8_t i = 0; i < listSize; i++) {
				rapidjson::Value contentValue = ReadInteger(buffer);
				listContent.PushBack(contentValue, allocator);
			}

			value.AddMember(rapidjson::Value("_Content", allocator), listContent, allocator);
			return value;
		}

		rapidjson::Value Parser::ReadObjectType(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::ObjectType)), allocator);
			value.AddMember(rapidjson::Value("_X", allocator), ReadInteger(buffer), allocator);
			value.AddMember(rapidjson::Value("_Y", allocator), ReadInteger(buffer), allocator);
			return value;
		}

		rapidjson::Value Parser::ReadObjectId(DataBuffer& buffer) {
			auto& allocator = mDocument.GetAllocator();

			rapidjson::Value value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", allocator), rapidjson::Value(static_cast<uint8_t>(Type::ObjectId)), allocator);
			value.AddMember(rapidjson::Value("_X", allocator), ReadInteger(buffer), allocator);
			value.AddMember(rapidjson::Value("_Y", allocator), ReadInteger(buffer), allocator);
			value.AddMember(rapidjson::Value("_Z", allocator), ReadInteger(buffer), allocator);
			return value;
		}

		// Packet
		Packet::Packet() : mDocument(), mAllocator(mDocument.GetAllocator()) {
			mDocument.SetObject();
			mStack = { &mDocument };
		}

		// NEW
		void Packet::put_integer(const std::string& label, uint64_t value) {
			AddMember(mStack.back(), label, rapidjson::Value(value).Move());
		}

		void Packet::put_string(const std::string& label, const std::string& value) {
			AddMember(mStack.back(), label, rapidjson::Value(value, mAllocator).Move());
		}

		void Packet::put_blob(const std::string& label, const uint8_t* data, uint64_t size) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Binary)), mAllocator);
			if (data != nullptr && size > 0) {
				std::string str(size, '\0');
				std::memcpy(&str[0], data, size);

				value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(str, mAllocator).Move(), mAllocator);
			} else {
				value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value("", mAllocator).Move(), mAllocator);
			}

			AddMember(mStack.back(), label, value);
		}

		void Packet::put_object_type(const std::string& label, uint16_t x, uint16_t y) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint8_t>(Type::ObjectType)), mAllocator);
			value.AddMember(rapidjson::Value("_X", mAllocator), rapidjson::Value(x).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Y", mAllocator), rapidjson::Value(y).Move(), mAllocator);

			AddMember(mStack.back(), label, value);
		}

		void Packet::put_object_id(const std::string& label, uint16_t x, uint16_t y, uint64_t z) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint8_t>(Type::ObjectId)), mAllocator);
			value.AddMember(rapidjson::Value("_X", mAllocator), rapidjson::Value(x).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Y", mAllocator), rapidjson::Value(y).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Z", mAllocator), rapidjson::Value(z).Move(), mAllocator);

			AddMember(mStack.back(), label, value);
		}

		void Packet::put_object_type(const std::string& label, const object_type& objectType) {
			put_object_type(label, std::get<0>(objectType), std::get<1>(objectType));
		}

		void Packet::put_object_id(const std::string& label, const object_id& objectId) {
			put_object_id(label, std::get<0>(objectId), std::get<1>(objectId), std::get<2>(objectId));
		}

		void Packet::push_integer_list(const std::string& label) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint8_t>(Type::IntegerList)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kArrayType).Move(), mAllocator);

			auto& newParent = AddMember(mStack.back(), label, value)["_Content"];
			mStack.push_back(&newParent);
		}

		void Packet::push_struct(const std::string& label) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Struct)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kObjectType).Move(), mAllocator);
			
			auto& newParent = AddMember(mStack.back(), label, value)["_Content"];
			mStack.push_back(&newParent);
		}

		void Packet::push_union(const std::string& label, uint32_t type) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Union)), mAllocator);
			value.AddMember(rapidjson::Value("_AddressMember", mAllocator), rapidjson::Value(type), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kObjectType).Move(), mAllocator);

			auto& newParent = AddMember(mStack.back(), label, value)["_Content"];
			mStack.push_back(&newParent);
		}

		void Packet::push_map(const std::string& label, Type keyType, Type valueType) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Map)), mAllocator);
			value.AddMember(rapidjson::Value("_KeyType", mAllocator), rapidjson::Value(static_cast<uint32_t>(keyType)), mAllocator);
			value.AddMember(rapidjson::Value("_ValueType", mAllocator), rapidjson::Value(static_cast<uint32_t>(valueType)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kObjectType).Move(), mAllocator);

			auto& newParent = AddMember(mStack.back(), label, value)["_Content"];
			mStack.push_back(&newParent);
		}

		void Packet::push_list(const std::string& label, Type type, bool isStub) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::List)), mAllocator);
			value.AddMember(rapidjson::Value("_ListType", mAllocator), rapidjson::Value(static_cast<uint32_t>(type)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kArrayType).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Stub", mAllocator), rapidjson::Value(isStub).Move(), mAllocator);

			auto& newParent = AddMember(mStack.back(), label, value)["_Content"];
			mStack.push_back(&newParent);
		}

		void Packet::pop() {
			if (mStack.size() > 1) {
				mStack.pop_back();
			}
		}

		// OLD
		void Packet::Write(DataBuffer& buffer) const {
			for (auto& member : mDocument.GetObject()) {
				Write(buffer, &member.name, member.value);
			}
		}

		void Packet::PutInteger(rapidjson::Value* parent, const std::string& label, uint64_t value) {
			AddMember(parent, label, rapidjson::Value(value).Move());
		}

		void Packet::PutString(rapidjson::Value* parent, const std::string& label, const std::string& value) {
			AddMember(parent, label, rapidjson::Value(value, mAllocator).Move());
		}

		void Packet::PutBlob(rapidjson::Value* parent, const std::string& label, const uint8_t* data, uint64_t size) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Binary)), mAllocator);
			if (data != nullptr && size > 0) {
				std::string str(size, '\0');
				std::memcpy(&str[0], data, size);

				value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(str, mAllocator).Move(), mAllocator);
			} else {
				value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value("", mAllocator).Move(), mAllocator);
			}

			AddMember(parent, label, value);
		}

		void Packet::PutObjectType(rapidjson::Value* parent, const std::string& label, uint16_t x, uint16_t y) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint8_t>(Type::ObjectType)), mAllocator);
			value.AddMember(rapidjson::Value("_X", mAllocator), rapidjson::Value(x).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Y", mAllocator), rapidjson::Value(y).Move(), mAllocator);

			AddMember(parent, label, value);
		}

		void Packet::PutObjectId(rapidjson::Value* parent, const std::string& label, uint16_t x, uint16_t y, uint64_t z) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint8_t>(Type::ObjectId)), mAllocator);
			value.AddMember(rapidjson::Value("_X", mAllocator), rapidjson::Value(x).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Y", mAllocator), rapidjson::Value(y).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Z", mAllocator), rapidjson::Value(z).Move(), mAllocator);

			AddMember(parent, label, value);
		}

		rapidjson::Value& Packet::CreateStruct(rapidjson::Value* parent, const std::string& label) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Struct)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kObjectType).Move(), mAllocator);
			return AddMember(parent, label, value)["_Content"];
		}

		rapidjson::Value& Packet::CreateUnion(rapidjson::Value* parent, const std::string& label, NetworkAddressMember addressMember) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Union)), mAllocator);
			value.AddMember(rapidjson::Value("_AddressMember", mAllocator), rapidjson::Value(static_cast<uint32_t>(addressMember)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kObjectType).Move(), mAllocator);
			return AddMember(parent, label, value)["_Content"];
		}

		rapidjson::Value& Packet::CreateList(rapidjson::Value* parent, const std::string& label, Type listType, bool isStub) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::List)), mAllocator);
			value.AddMember(rapidjson::Value("_ListType", mAllocator), rapidjson::Value(static_cast<uint32_t>(listType)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kArrayType).Move(), mAllocator);
			value.AddMember(rapidjson::Value("_Stub", mAllocator), rapidjson::Value(isStub).Move(), mAllocator);
			return AddMember(parent, label, value)["_Content"];
		}

		rapidjson::Value& Packet::CreateMap(rapidjson::Value* parent, const std::string& label, Type keyType, Type valueType) {
			auto value = rapidjson::Value(rapidjson::kObjectType);
			value.AddMember(rapidjson::Value("_Type", mAllocator), rapidjson::Value(static_cast<uint32_t>(Type::Map)), mAllocator);
			value.AddMember(rapidjson::Value("_KeyType", mAllocator), rapidjson::Value(static_cast<uint32_t>(keyType)), mAllocator);
			value.AddMember(rapidjson::Value("_ValueType", mAllocator), rapidjson::Value(static_cast<uint32_t>(valueType)), mAllocator);
			value.AddMember(rapidjson::Value("_Content", mAllocator), rapidjson::Value(rapidjson::kObjectType).Move(), mAllocator);
			return AddMember(parent, label, value)["_Content"];
		}

		rapidjson::Value& Packet::AddMember(rapidjson::Value* parent, const std::string& name, rapidjson::Value& value) {
			if (!parent) {
				parent = &mDocument;
			}

			assert(parent->IsObject() || parent->IsArray());
			if (parent->IsObject()) {
				parent->AddMember(rapidjson::Value(name, mAllocator).Move(), value, mAllocator);
				return (--parent->MemberEnd())->value;
			} else {
				auto index = parent->GetArray().Size();
				parent->PushBack(value, mAllocator);
				return (*parent)[index];
			}
		}

		void Packet::Write(DataBuffer& buffer, const rapidjson::Value* name, const rapidjson::Value& value) const {
			if (value.IsObject()) {
				Type type = static_cast<Type>(value["_Type"].GetUint());
				WriteType(buffer, type, name, value);
			} else if (value.IsArray()) {
				for (auto& child : value.GetArray()) {
					Write(buffer, nullptr, child);
				}
			} else if (value.IsString()) {
				WriteType(buffer, Type::String, name, value);
			} else {
				WriteType(buffer, Type::Integer, name, value);
			}
		}

		void Packet::WriteType(DataBuffer& buffer, Type type, const rapidjson::Value* name, const rapidjson::Value& value) const {
			if (name) {
				WriteHeader(buffer, name->GetString(), type);
			}

			switch (type) {
				case Type::Integer: {
					if (value.IsString()) {
						uint64_t intValue = std::stoull(value.GetString(), nullptr, 0);
						buffer.encode_tdf_integer(intValue);
					} else {
						buffer.encode_tdf_integer(value.GetUint64());
					}
					break;
				}

				case Type::String: {
					std::string str = value.GetString();
					uint64_t strLen = str.length();

					buffer.encode_tdf_integer(strLen + 1);
					buffer.write<char>(&str[0], strLen);
					buffer.write<char>(0);
					break;
				}

				case Type::Binary: {
					std::string str = value["_Content"].GetString();
					uint64_t strLen = str.length();

					buffer.encode_tdf_integer(strLen);
					buffer.write<char>(&str[0], strLen);
					break;
				}

				case Type::IntegerList: {
					auto content = value["_Content"].GetArray();

					uint8_t contentLength = static_cast<uint8_t>(content.Size());
					buffer.write<uint8_t>(contentLength);

					for (uint8_t i = 0; i < contentLength; ++i) {
						buffer.encode_tdf_integer(content[i].GetUint64());
					}
					break;
				}

				case Type::Union: {
					uint32_t addressMember = value["_AddressMember"].GetUint();
					buffer.write<uint8_t>(addressMember);
					if (static_cast<NetworkAddressMember>(addressMember) != NetworkAddressMember::Unset) {
						for (auto& member : value["_Content"].GetObject()) {
							Write(buffer, &member.name, member.value);
						}
					}
					break;
				}

				case Type::Struct: {
					for (auto& member : value["_Content"].GetObject()) {
						Write(buffer, &member.name, member.value);
					}
					buffer.write<uint8_t>(0);
					break;
				}

				case Type::List: {
					auto content = value["_Content"].GetArray();

					uint32_t listType = value["_ListType"].GetUint();
					buffer.write<uint8_t>(listType);

					uint8_t contentLength = static_cast<uint8_t>(content.Size());
					buffer.write<uint8_t>(contentLength);

					bool isStub = value["_Stub"].GetBool();
					if (isStub) {
						buffer.write<uint8_t>(0x02);
						for (rapidjson::SizeType i = 0; i < content.Size(); ++i) {
							WriteType(buffer, static_cast<Type>(listType), nullptr, content[i]);
						}
					} else {
						for (uint8_t i = 0; i < contentLength; ++i) {
							WriteType(buffer, static_cast<Type>(listType), nullptr, content[i]);
						}
					}
					break;
				}

				case Type::Map: {
					auto content = value["_Content"].GetObject();

					uint32_t keyType = value["_KeyType"].GetUint();
					buffer.write<uint8_t>(keyType);

					uint32_t valueType = value["_ValueType"].GetUint();
					buffer.write<uint8_t>(valueType);

					uint8_t contentLength = static_cast<uint8_t>(content.MemberCount());
					buffer.write<uint8_t>(contentLength);

					for (auto& member : content) {
						WriteType(buffer, static_cast<Type>(keyType), nullptr, member.name);
						WriteType(buffer, static_cast<Type>(valueType), nullptr, member.value);
					}
					break;
				}

				case Type::ObjectType: {
					buffer.encode_tdf_integer(value["_X"].GetUint64());
					buffer.encode_tdf_integer(value["_Y"].GetUint64());
					break;
				}

				case Type::ObjectId: {
					buffer.encode_tdf_integer(value["_X"].GetUint64());
					buffer.encode_tdf_integer(value["_Y"].GetUint64());
					buffer.encode_tdf_integer(value["_Z"].GetUint64());
					break;
				}

				default:
					logger::error("Unimplemented write type: " + static_cast<int>(type));
					break;
			}
		}
	}
}
