#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H

#include <set>
#include <model/spreadsheet.h>

/**
 * The main spreadsheet controller that acts as the "leader" for all other controllers.
 */
class spreadsheet_controller {

    /**
     * All loaded spreadsheets.
     */
    std::set<spreadsheet> spreadsheets_;

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H
