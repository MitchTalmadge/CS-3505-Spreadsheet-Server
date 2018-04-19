#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H

#include <string>
#include <vector>
#include <model/spreadsheet.h>
#include <controller/data_container.h>

/**
 * Controller for all the available and loaded spreadsheets.
 */
class spreadsheet_controller {

 private:

  /**
   * The path to the directory where save files will be located.
   */
  static const std::string FILE_DIR_PATH;

  /**
   * Whether the instance is currently alive. (Has not been shut down).
   */
  static bool instance_alive_;

  /**
   * The number of ticks left until all spreadsheets should be saved.
   * Initialized to 3 minutes worth of ticks.
   */
  int save_countdown_ = 18000;

  /**
   * Contains all spreadsheets which are active, meaning they have been loaded by a client.
   * Maps spreadsheet file names to spreadsheet instances.
   */
  std::map<const std::string, spreadsheet *> active_spreadsheets_;

  /**
   * Map from spreadsheet to associated sockets.
   */
  std::map<std::string, std::vector<int>> spreadsheets_to_sockets_;

  /**
   * Map from sockets to associated spreadsheet.
   */
  std::map<int, std::string> sockets_to_spreadsheets_;

  /**
   * The data container that interfaces between the network controller and spreadsheet controller.
   */
  data_container &data_container_ = data_container::get_instance();

  /**
   * Private constructor for singleton pattern.
   */
  spreadsheet_controller();

  /**
   * Private destructor for singleton pattern.
   */
  ~spreadsheet_controller();

  /**
   * The work-loop for all active spreadsheets.
   */
  void work();

  /**
   * Parses an inbound packet for a given spreadsheet.
   * @param packet The packet to parse.
   * @param spreadsheet_name The spreadsheet file name.
   * @param sheet The spreadsheet instance.
   */
  void parse_inbound_packet(inbound_packet &packet);

  /**
   * Sends an outbound packet to all the sockets mapped to a given spreadsheet.
   * @param spreadsheet_name The name of the spreadsheet for which to send packets to all sockets.
   * @param packet The packet to send.
   */
  void send_packet_to_all_sockets(const std::string &spreadsheet_name, outbound_packet &packet) const;

  /**
   * Saves all active spreadsheets one at a time.
   */
  void save_all_spreadsheets() const;

 public:

  /**
   * @return The singleton instance of this controller.
   */
  static spreadsheet_controller &get_instance();

  /**
   * Shuts down and disposes of this singleton.
   */
  static void shut_down();

  /**
   * Deleted copy constructor since this is a singleton.
   */
  spreadsheet_controller(spreadsheet_controller const &) = delete;

  /**
   * Deleted assignment operator since this is a singleton.
   */
  void operator=(spreadsheet_controller const &)  = delete;

  /**
   * Determines if the given cell name is formatted correctly.
   *
   * A valid cell name is one which starts with a letter or underscore,
   * and is followed by zero or more letters, underscores, or numbers.
   * @param cell_name The cell name to check.
   * @return True if the cell name is valid, false otherwise.
   */
  static bool is_valid_cell_name(const std::string &cell_name);

  /**
   * Normalizes the given cell name for consistency.
   *
   * All cell names will be converted to uppercase.
   *
   * @param cellName The name of the cell to normalize.
   * @return The normalized cell name.
   */
  static std::string normalize_cell_name(std::string cellName);

  /**
   * @return The names of all existing spreadsheets.
   */
  std::vector<std::string> get_spreadsheet_names() const;

};

#endif
