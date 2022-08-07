#include "charFollow.h"


int nbDiff = 0;


int return_with_error(int ret, const char *msg)
{  
    perror(msg);
    return ret;
}

/*  returns 0 on success
 *  -1 on failure
 */
int add_final_chars(FILE *file, CharF cf, char *replacer, char *toBeReplace)
{
    int i = 0, fd = 0;
    off_t pos = 0;

    while (CF_is_empty(cf) == False)
    {
        if (CF_contains(cf, toBeReplace) == True)
        {
            nbDiff++;
            for (i = 0; i < strlen(toBeReplace); ++i)
            {
                cf = CF_remove(cf);
            }

            for (i = 0; i < strlen(replacer); i++)
                if (fputc(replacer[i], file) == EOF)
                    return -1;

            continue;
        }
        if (fputc(cf->c, file) == EOF)
            return -1;

        cf = CF_remove(cf);
    }

    // remove the file end (if replacer < toBeReplaced)
    if ((pos = ftello(file)) == -1)
        return return_with_error(-1, "Error: cannot get file position\n");

    if ((fd = fileno(file)) == -1)
        return return_with_error(-1, "Error: cannot get file descriptor from FILE\n");

    if (ftruncate(fd, pos) == -1)
        return return_with_error(-1, "Error: cannot cut the end of the file\n");

    return 0;
}


int main(int argc, char**argv)
{
    CharF cf = CF_create();

    char *toBeReplace;
    char *replacer;
    char *filename;
    int i = 0;
    char c;

    FILE *file = NULL;
    int betweenThem = 0;
    int diff = 0;

    int toAdd = 0;

    int max = 0;
    size_t CF_len = 0;

    Bool replacing = True;

    if (argc < 4)
    {
        printf("Utilisation de la command : replace [to be replace] [replacer] [filepPath]\nLes espaces séparent les arguments et ne sont pas compris dans les paramètres\n");
        return 0;
    }
    toBeReplace = argv[1];
    replacer = argv[2];
    filename = argv[3];

    betweenThem = strlen(replacer) - strlen(toBeReplace);

    printf("je vais remplacer tous les [%s] en [%s] du fichier [%s]\n", toBeReplace, replacer, filename);

    max = strlen(replacer) > strlen(toBeReplace) ? strlen(replacer) : strlen(toBeReplace);
    CF_len = 2 * max;


    if ((file = fopen(filename, "r+")) == NULL)
    {
        fprintf(stderr, "Error: cannot open file\n");
        exit(EXIT_FAILURE);
    }


    for (i = 0; i < CF_len; ++i)
    {
        if((c = fgetc(file)) == EOF)
        {
            rewind(file);
            add_final_chars(file, cf, replacer, toBeReplace);
            return(EXIT_SUCCESS);
        }
        cf = CF_add(cf, c);
    }
    rewind(file);
    
    while(replacing == True)
    {

        if (CF_is_empty(cf))
        {
            replacing = False;
            break;
        }

        // string to be replace not detected
        if (CF_contains(cf, toBeReplace) == False)
        {
            if (fputc(cf->c, file) == EOF)
                return(EXIT_FAILURE);
            
            cf = CF_remove(cf);
            // get char after the last one in cf
            if (fseek(file, CF_len - diff - 1, SEEK_CUR) != 0)
            {
                fprintf(stderr, "Error: cannot move file pointer\n");
                exit(EXIT_FAILURE);
            }
            //showChar(file, "je suis ici après avoir bougé");

            if ((c = fgetc(file)) == EOF)
            {
                if (fseek(file, -CF_len + diff  + 1, SEEK_CUR) == EOF)
                    return(EXIT_FAILURE);

                if (add_final_chars(file, cf, replacer, toBeReplace) != 0)
                {
                    fprintf(stderr, "Error: cannot finalize file\n");
                    exit(EXIT_FAILURE);
                }
                cf = CF_create();
                break;
            }
            
            cf = CF_add(cf, c);

            if (fseek(file, -CF_len + diff - 1 + 1, SEEK_CUR) == EOF)
                return(EXIT_FAILURE);

            continue;
        }

        diff += betweenThem;
        nbDiff++;


        // go to the after char that is at the end of the CharFollow struct 
        if (fseek(file, CF_len - diff + betweenThem, SEEK_CUR) != 0)  // - strlen(replacer)
        {
            fprintf(stderr, "Error: cannot move file pointer\n");
            exit(EXIT_FAILURE);
        }


        // increase the size of Cf_len (when strlen(rpl) > strlen(TBR))
        if (strlen(replacer) == max)
            toAdd = abs(betweenThem);

        // adding the next chars in the CharFollow struct
        for (i = 0; i < strlen(toBeReplace) + toAdd; ++i)
        {
            if ((c = fgetc(file)) == EOF)
            {                
                if (fseek(file, - CF_len - i + diff - betweenThem, SEEK_CUR) == EOF) // + strlen(replacer)
                    return(EXIT_FAILURE);

                if (add_final_chars(file, cf, replacer, toBeReplace) != 0)
                {
                    fprintf(stderr, "Error: cannot finalize file\n");
                    exit(EXIT_FAILURE);
                }
                cf = CF_create();
                replacing = False;
                break;
            }
            
            cf = CF_add(cf, c);
        }

        if (replacing == False)
            break;

        // return after the char to continue to replace
        if (fseek(file, -CF_len - strlen(toBeReplace) + diff - betweenThem - toAdd, SEEK_CUR) == EOF)  // + strlen(replacer) 
            return(EXIT_FAILURE);

        // remove the replacer from the CharFollow struct
        for (i = 0; i < strlen(toBeReplace); ++i)
            cf = CF_remove(cf);

        // place whole string into the file
        for (i = 0; i < strlen(replacer); ++i)
            if (fputc(replacer[i], file) == EOF)
                return(EXIT_FAILURE);

        CF_len += toAdd;

    }

    printf("%d occurences ont été trouvés !!\n", nbDiff);

    return 0;

}