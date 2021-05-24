#ifndef FA_KURS_CLIENT_CODE_H
#define FA_KURS_CLIENT_CODE_H

#include <forward_list>

#define LIST_IMPLEMENTATION forward_list
#define BAD_ACCESS "You cannot access this operation: "

int clientCode(ofstream &fp, int err_code)
{
    SuperGenerator<FieldRusLot, LIST_IMPLEMENTATION> *super_gen = new SuperGeneratorRusLot<LIST_IMPLEMENTATION>();
    Game<FieldRusLot, LIST_IMPLEMENTATION> *game=super_gen->gen_game->getGame(0, 3, 20000, 0.8, *super_gen->gen_lot,
                                                                              *super_gen->gen_tic);
    cout << "Available operations:\n"
            "'p' - enter process mode and id of lot to process. In zero mode id is useless;\n"
            "'o' - print winners;\n"
            "'f' - write processed data to output_file;\n"
            "'s' - search specific tickets in output_file;\n"
            "'c' - finish work and exit program.";
    char operation;
    int process_mode, number;
    bool proceed=true, p_not_blocked=true, s_not_blocked=false, o_f_not_blocked=false;
    while(proceed)
    {
        cout << "\n\nEnter operation:\n";
        cin >> operation;
        switch(operation)
        {
            case 'p':
                if(p_not_blocked)
                {
                    cin >> process_mode >> number;
                    while((err_code = game->processGame(process_mode, number, err_code)) == -1)
                        cin >> process_mode;
                    if(err_code)
                    {
                        err_code=1;
                        p_not_blocked=false;
                        break;
                    }
                    o_f_not_blocked=true;
                }
                else
                    cout << BAD_ACCESS"Last calculation ended with errors.\n";
                break;

            case 'o':
                if(o_f_not_blocked)
                    game->printAllWinners();
                else
                    cout << BAD_ACCESS"Game has not been processed or processing has ended with errors.\n";
                break;

            case 'f':
                if(o_f_not_blocked)
                {
                    fp << *game;
                    s_not_blocked=true;
                    cout << "Done.\n";
                }
                else
                    cout << BAD_ACCESS"Game has not been processed or processing has ended with errors.\n";
                break;

            case 's':
                if(s_not_blocked)
                {
                    if(!searchTickets(*game, super_gen->gen_tic->getTicket(0, 0, -1), err_code))
                        err_code=1;
                }
                else
                    cout << BAD_ACCESS"Processed data has not been written to file.\n";
                break;

            case 'c':
                proceed=false;
                break;

            default:
                cout << "No such operation, try again.\n";
        }
    }
    delete super_gen; delete game;

    return errCheck_ClientCode(err_code, 0);
}

#endif
