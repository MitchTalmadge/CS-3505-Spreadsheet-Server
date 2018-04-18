#include <gtest/gtest.h>
#include <model/packet/inbound/inbound_packet_factory.h>
#include <model/packet/inbound/inbound_register_packet.h>

const std::string EOT = std::to_string('\3');

TEST(inbound_packet_factory, from_raw_message__register) {
  auto packet = inbound_packet_factory::from_raw_message(0, "register " + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::REGISTER)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_register_packet *>(packet));

  // Dispose of packet.
  delete packet;
}