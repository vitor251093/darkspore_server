
#ifndef _BLAZE_CLIENT_HEADER
#define _BLAZE_CLIENT_HEADER

// Include
#include "types.h"
#include "functions.h"

#include "../network/client.h"

#include "../sporenet/user.h"

#include <boost/asio/ssl.hpp>

// Blaze
namespace Blaze {
	// Client
	class Client : public Network::Client {
		public:
			Client(boost::asio::io_context& io_service, boost::asio::ssl::context& context);

			auto& get_socket() { return mSocket.lowest_layer(); }

			auto get_id() const { return mId; }

			const SporeNet::UserPtr& get_user() const { return mUser; }
			void set_user(const SporeNet::UserPtr& user) { mUser = user; }

			const auto& get_request() const { return mParser.GetDocument(); }

			auto& data() { return mData; }
			const auto& data() const { return mData; }

			void start();

			void notify(Header header, const DataBuffer& buffer);
			void reply(Header header);
			void reply(Header header, const DataBuffer& buffer);

			//
			void notify(Header&& header, const TDF::Packet& packet);
			void reply(Header&& header, const TDF::Packet& packet);
			//

			void handle_handshake(const boost::system::error_code& error);
			void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
			void handle_write(const boost::system::error_code& error);

		private:
			void send(const Header& header, const DataBuffer& buffer);

			void parse_packets();

		protected:
			boost::asio::ssl::stream<boost::asio::ip::tcp::socket> mSocket;

			TDF::Parser mParser;

			std::vector<DataBuffer> mWriteBuffers;

			SporeNet::UserPtr mUser;

			ClientData mData;
			
			uint32_t mCurrentMessageId;
			uint32_t mId;
	};
}

#endif
