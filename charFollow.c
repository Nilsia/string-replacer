#include "charFollow.h"

CharF CF_create()
{
    return NULL;
}

Bool CF_is_empty(CharF cf)
{
    if (cf == NULL)
        return True;
    return False;
}


CharF CF_get_last(CharF cf)
{
    CharFollow *temp;

    if (CF_is_empty(cf))
        return NULL;

    temp = cf;
    while (temp->charFollowing != NULL)
    {
        temp = temp->charFollowing;
    }
    return temp;
}


CharF CF_add(CharF cf, char c)
{
    CharFollow *element;
    CharFollow *temp;

    element = malloc(sizeof(*element));

    if (element == NULL) 
    {
        fprintf(stderr, "Erreur allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    if (CF_is_empty(cf))
    {
        element->c = c;
        element->charFollowing = NULL;
        return element;
    }

    temp = CF_get_last(cf);
    element->c = c;
    element->charFollowing = NULL;

    temp->charFollowing = element;

    return cf;
}


CharF CF_remove(CharF cf)
{
    CharFollow *element;

    if (CF_is_empty(cf))
    {
        printf("Rien à supprimer la liste des élements à ajouter est vide\n");
        return cf;
    }

    element = cf->charFollowing;

    free(cf);

    return element;
}


CharF CF_clear(CharF cf)
{
    
    if (CF_is_empty(cf))
        return CF_create();

    while (!CF_is_empty(cf))
        cf = CF_remove(cf);

    return CF_create();

}


void CF_print(CharF cf)
{
    if (CF_is_empty(cf))
    {
        printf("La liste de char est vide\n");
        return;
    }

    printf("printing CharFollow : \n");
    while (!CF_is_empty(cf))
    {
        printf("%c", cf->c);
        cf = cf->charFollowing;
    }
    printf("\n");
}


Bool CF_contains(CharF cf, char* str)
{
    int i = 0;
    if (CF_is_empty(cf))
        return False;

    for (i = 0; i < strlen(str); ++i)
    {
        if (CF_is_empty(cf))
            return False;

        if (str[i] != cf->c)
            return False;

        cf = cf->charFollowing;
    }

    return True;
}



