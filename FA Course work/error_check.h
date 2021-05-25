#ifndef FA_KURS_ERROR_CHECK_H
#define FA_KURS_ERROR_CHECK_H

#define NON_CRITICAL "\nNON CRITICAL: "
#define CRITICAL     "\nCRITICAL: "

using namespace std;

static int err_code;            // Замена errno.

int errCheck_Components(unsigned int additional_information)          // Можно было упаковать в классы, но, по-моему, лучше так.
{
    switch(err_code)
    {
        case 1:
            cerr << NON_CRITICAL"Unable to find Lot with id " << additional_information << " in Game!\nProcessing of this lot will not begin.\n\n";
            break;
        case 2:
            cerr << NON_CRITICAL"Lot with id " << additional_information << " has been already processed.\nReprocessing is prohibited\n\n";
            break;
    }
}

int errCheck_ClientCode()
{
    switch(err_code)
    {
        case 1:
            cerr << NON_CRITICAL"Game processing operation ended with errors.\n\n";
            break;
        case 2:
            cerr << NON_CRITICAL"Search operation has returned nullptr.\n\n";
            break;
    }
    if(err_code)
        return 2;
    return 0;
}

void errCheck_Main()
{
    switch(err_code)
    {
        case 0:
            exit(0);
        case 1:
            cerr << CRITICAL"Failed to open output file.";          // Обьекты, наследуемые от класса исключений, при критических ошибках не используются.
            exit(1);
        case 2:
            cerr << NON_CRITICAL"Program has ended with some errors.";
            exit(2);
    }
}

#endif
