#include <gtest/gtest.h>
#include <model/packet/inbound/inbound_packet_factory.h>
#include <model/packet/inbound/inbound_register_packet.h>
#include <model/packet/inbound/inbound_disconnect_packet.h>
#include <model/packet/inbound/inbound_load_packet.h>
#include <model/packet/inbound/inbound_ping_packet.h>
#include <model/packet/inbound/inbound_ping_response_packet.h>
#include <model/packet/inbound/inbound_edit_packet.h>
#include <model/packet/inbound/inbound_focus_packet.h>
#include <model/packet/inbound/inbound_unfocus_packet.h>
#include <model/packet/inbound/inbound_undo_packet.h>
#include <model/packet/inbound/inbound_revert_packet.h>

const std::string EOT = std::string(1, '\3');

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

TEST(inbound_packet_factory, from_raw_message__disconnect) {
  auto packet = inbound_packet_factory::from_raw_message(0, "disconnect " + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::DISCONNECT)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_disconnect_packet *>(packet));

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__load) {
  auto packet = inbound_packet_factory::from_raw_message(0, "load test" + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::LOAD)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_load_packet *>(packet));

  // Check spreadsheet name.
  auto load_packet = dynamic_cast<inbound_load_packet *>(packet);
  EXPECT_EQ("test", load_packet->get_spreadsheet_name());

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__ping) {
  auto packet = inbound_packet_factory::from_raw_message(0, "ping " + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::PING)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_ping_packet *>(packet));

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__ping_response) {
  auto packet = inbound_packet_factory::from_raw_message(0, "ping_response " + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::PING_RESPONSE)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_ping_response_packet *>(packet));

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__edit) {
  auto packet = inbound_packet_factory::from_raw_message(0, "edit Z99:=A10 + (5 * 10 - 100)" + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::EDIT)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_edit_packet *>(packet));

  // Check cell name and contents.
  auto edit_packet = dynamic_cast<inbound_edit_packet *>(packet);
  EXPECT_EQ("Z99", edit_packet->get_cell_name());
  EXPECT_EQ("=A10 + (5 * 10 - 100)", edit_packet->get_cell_contents());

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__edit__colon) {
  auto packet = inbound_packet_factory::from_raw_message(0, "edit Z99:lol:hax" + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::EDIT)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_edit_packet *>(packet));

  // Check cell name and contents.
  auto edit_packet = dynamic_cast<inbound_edit_packet *>(packet);
  EXPECT_EQ("Z99", edit_packet->get_cell_name());
  EXPECT_EQ("lol:hax", edit_packet->get_cell_contents());

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__focus) {
  auto packet = inbound_packet_factory::from_raw_message(0, "focus A10" + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::FOCUS)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_focus_packet *>(packet));

  // Check cell name.
  auto focus_packet = dynamic_cast<inbound_focus_packet *>(packet);
  EXPECT_EQ("A10", focus_packet->get_cell_name());

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__unfocus) {
  auto packet = inbound_packet_factory::from_raw_message(0, "unfocus " + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::UNFOCUS)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_unfocus_packet *>(packet));

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__undo) {
  auto packet = inbound_packet_factory::from_raw_message(0, "undo " + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::UNDO)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_undo_packet *>(packet));

  // Dispose of packet.
  delete packet;
}

TEST(inbound_packet_factory, from_raw_message__revert) {
  auto packet = inbound_packet_factory::from_raw_message(0, "revert A5" + EOT);

  // Check if packet is not null.
  ASSERT_TRUE(packet);

  // Check if packet type is correct.
  EXPECT_TRUE(packet->get_packet_type() == inbound_packet::REVERT)
            << "Packet type was: " << packet->get_packet_type();

  // Check if packet class is correct.
  EXPECT_TRUE(dynamic_cast<inbound_revert_packet *>(packet));

  // Check cell name.
  auto revert_packet = dynamic_cast<inbound_revert_packet *>(packet);
  EXPECT_EQ("A10", revert_packet->get_cell_name());

  // Dispose of packet.
  delete packet;
}