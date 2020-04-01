
#ifndef _BLAZE_TDF_HEADER
#define _BLAZE_TDF_HEADER

// Include
#include "databuffer.h"
#include "types.h"

#include <rapidjson/document.h>
#include <memory>
#include <functional>
#include <tuple>

// Blaze
namespace Blaze {
	using object_type = std::tuple<uint16_t, uint16_t>;
	using object_id = std::tuple<uint16_t, uint16_t, uint64_t>;

	void Log(const rapidjson::Document& document);

	// TODO: less json dumbery
	// TDF
	namespace TDF {
		using ValuePtr = std::shared_ptr<class Value>;

		// Type
		enum class Type {
			Integer,
			String,
			Binary,
			Struct,
			List,
			Map,
			Union,
			IntegerList,
			ObjectType, // (u16, u16)?
			ObjectId, // (u16, u16, u64)?
			Float,
			TimeValue,
			Invalid = 0xFF
		};

		// Header
		struct Header {
			std::string label;
			Type type;
		};

		uint32_t CompressLabel(const std::string& label);
		std::string DecompressLabel(uint32_t label);

		Header ReadHeader(DataBuffer& buffer);
		void WriteHeader(DataBuffer& buffer, const Header& header);
		void WriteHeader(DataBuffer& buffer, const std::string& label, Type type);
		void WriteUnionHeader(DataBuffer& buffer, const std::string& label, NetworkAddressMember activeMember);

		uint64_t read_integer(DataBuffer& buffer);
		void WriteInteger(DataBuffer& buffer, const std::string& label, uint64_t value);

		std::string read_string(DataBuffer& buffer);
		void WriteString(DataBuffer& buffer, const std::string& label, const std::string& value);

		// Parser
		class Parser {
			public:
				Parser();

				void Read(DataBuffer& buffer);

				auto& GetDocument() { return mDocument; }
				const auto& GetDocument() const { return mDocument; }

			private:
				void ReadValue(DataBuffer& buffer, rapidjson::Value& parent);

				rapidjson::Value ReadInteger(DataBuffer& buffer);
				rapidjson::Value ReadString(DataBuffer& buffer);
				rapidjson::Value ReadBlob(DataBuffer& buffer);
				rapidjson::Value ReadStruct(DataBuffer& buffer);
				rapidjson::Value ReadUnion(DataBuffer& buffer);
				rapidjson::Value ReadList(DataBuffer& buffer);
				rapidjson::Value ReadMap(DataBuffer& buffer);
				rapidjson::Value ReadIntegerList(DataBuffer& buffer);
				rapidjson::Value ReadObjectType(DataBuffer& buffer);
				rapidjson::Value ReadObjectId(DataBuffer& buffer);

			private:
				rapidjson::Document mDocument;

				Header mHeader;
		};

		// Packet
		class Packet {
			public:
				Packet();
				
				// NEW
				void put_integer(const std::string& label, uint64_t value);
				void put_string(const std::string& label, const std::string& value);
				void put_blob(const std::string& label, const uint8_t* data, uint64_t size);
				void put_object_type(const std::string& label, uint16_t x, uint16_t y);
				void put_object_id(const std::string& label, uint16_t x, uint16_t y, uint64_t z);

				void put_object_type(const std::string& label, const object_type& objectType);
				void put_object_id(const std::string& label, const object_id& objectId);

				void push_integer_list(const std::string& label);
				void push_struct(const std::string& label);
				void push_union(const std::string& label, uint32_t type);
				void push_map(const std::string& label, Type keyType, Type valueType);
				void push_list(const std::string& label, Type type, bool isStub = false);

				void pop();

				template<typename T>
				std::enable_if_t<std::is_enum_v<T>, void> put_integer(const std::string& label, T value) {
					put_integer(label, static_cast<uint64_t>(value));
				}

				template<typename T>
				std::enable_if_t<std::is_enum_v<T>, void> push_union(const std::string& label, T value) {
					push_union(label, static_cast<uint32_t>(value));
				}

				// OLD
				void Write(DataBuffer& buffer) const;

				void PutInteger(rapidjson::Value* parent, const std::string& label, uint64_t value);
				void PutString(rapidjson::Value* parent, const std::string& label, const std::string& value);
				void PutBlob(rapidjson::Value* parent, const std::string& label, const uint8_t* data, uint64_t size);
				void PutObjectType(rapidjson::Value* parent, const std::string& label, uint16_t x, uint16_t y);
				void PutObjectId(rapidjson::Value* parent, const std::string& label, uint16_t x, uint16_t y, uint64_t z);

				rapidjson::Value& CreateStruct(rapidjson::Value* parent, const std::string& label);
				rapidjson::Value& CreateUnion(rapidjson::Value* parent, const std::string& label, NetworkAddressMember addressMember);
				rapidjson::Value& CreateList(rapidjson::Value* parent, const std::string& label, Type listType, bool isStub = false);
				rapidjson::Value& CreateMap(rapidjson::Value* parent, const std::string& label, Type keyType, Type valueType);

				template<typename T>
				std::enable_if_t<std::is_enum_v<T>, void> PutInteger(rapidjson::Value* parent, const std::string& label, T value) {
					PutInteger(parent, label, static_cast<uint64_t>(value));
				}

			private:
				rapidjson::Value& AddMember(rapidjson::Value* parent, const std::string& name, rapidjson::Value& value);

				void Write(DataBuffer& buffer, const rapidjson::Value* name, const rapidjson::Value& value) const;
				void WriteType(DataBuffer& buffer, Type type, const rapidjson::Value* name, const rapidjson::Value& value) const;

			private:
				rapidjson::Document mDocument;
				rapidjson::Document::AllocatorType& mAllocator;

				std::vector<rapidjson::Value*> mStack;
		};
	}
}

#endif
