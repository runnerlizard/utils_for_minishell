#include <stdlib.h>
#include <stdio.h>

size_t	ft_strlen(const char *s);

char	*ft_strchr(const char *s, int c);

char	*ft_substr(char const *s, unsigned int start, size_t len);

static int rotate_quotes_and_text(char *s, int i)
{
    int j;

    j = 0;
    while (s[i] != '<' && s[i] != '>' && s[i] != ' ' && (s[i]))
    {
        while (s[i] == 34 || s[i] == 39)
        {
            if (s[i] == 34 && ft_strchr(&s[i], 34) != NULL)
                while (s[++i] != 34)
                {}
            if (s[i] == 39 && ft_strchr(&s[i], 39) != NULL)
                while (s[++i] != 39)
                {}
            i++;
            j++;
        }
        if (j == 0)
            i++;
        j = 0;
    }
    return (i);
}

static int count_args(char *s)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (s[i])
    {
        while (s[i] == ' ')
            i++;
        if (s[i] == '>' || s[i] == '<')
        {
            while (s[i] == '<' || s[i] == '>')
                i++;
            while (s[i] == ' ')
                i++;
            i = rotate_quotes_and_text(s, i);
        }
        else
        {
            i = rotate_quotes_and_text(s, i);
            j++;
        }
    }
    return (j);
}

static char *get_arg(char *s, int *k)
{
    int i;
    char    *ret;

    i = rotate_quotes_and_text(s, *k);
    ret = malloc(sizeof(char) * (i - *k + 1));
    if (ret == NULL)
        exit (1); //TODO free all
    i = -1;
    while (s[*k] != ' ' && s[*k] != '<' && s[*k] != '>' && (s[*k]))
    {
        if (s[*k] == 34 && ft_strchr(&s[*k], 34) != NULL)
        {
            while (s[++*k] != 34)
                ret[++i] = s[*k];
            *k++;
            continue;
        }
        if (s[*k] == 39 && ft_strchr(&s[*k], 39) != NULL)
        {
            while (s[++*k] != 39)
                ret[++i] = s[*k];
            *k++;
            continue;
        }
        if (s[*k] != ' ' && s[*k] != '>' && s[*k] != '<')
            ret[++i] = s[*k++];
    }
    while (s[*k] == ' ')
        *k++;
    ret[++i] = 0;
    return (ret);
}

static int  redirect_create_outputs(char *s, int k)
{
    int fd;


//TODO create all outputs but first command
//TODO if input file just created handle it like empty
    if (s[k] == '<')
    {
        if (s[k + 1] == '<')
        {
            if (s[k + 2] == '>')
                //free all syntax error
            else if (s[k + 2] == '<')
                //free all syntax error
            //get filename
            fd = open("", O_RDONLY); // some mode
        }
        else if (s[k + 1] == ' ')
        {
            if (s[k + 2] == '<')
                //free all syntax error
            else if (s[k + 2] == '>')
                //free all syntax error
        }
        else
            //get filename
            fd = open("", O_RDONLY);
        if (fd < 0)
            //free all
        if (dup2(fd, 0) < 0)
            // free all 
    }
    if (s[k] == '<' && s[k + 1] == '>')
    {
        //get filename
        fd = open("", O_TRUNC | O_CREATE | O_RDWR, 0664);
    }
    if (s[k] == '>')
    {
        if (s[++k] == '<')
        {
            if (s[k + 2] == '>')
                //free all syntax error
            else if (s[k + 2] == '<')
                //free all syntax error
            //get filename
            // some mode free all syntax error
        }
        else if (s[k] == '>')
        {
            //get filename
            fd = open("", O_TRUNC | O_CREATE | O_RDWR | O_APPEND 0664);
        }
        else if (s[k] == ' ')
        {
            if (s[k + 1] == '<')
                //free all syntax error
            else if (s[k + 1] == '>')
                //free all syntax error
        }
        else
            //get filename
            fd = open("", O_RDONLY);
        if (fd < 0)
            //free all
        if (dup2(fd, 1) < 0)
            //free all 
    }
}

char    **handle_redirect(char *s)
{
    char    **ret;
    int     n;
    int     j;
    int     i;
    int     k;

    n = count_args(s);
    ret = malloc(sizeof(char *) * (n + 1));
    if (ret == NULL)
        exit (1);//TODO free all
    k = -1;
    j = -1;
    while (s[++k])
    {
        if (s[k] == '>' || s[k] == '<')
            k = redirect_create_outputs(s, k);
        else
            ret[++j] = get_arg(s, *k);
    }
    return (ret);
}

int main(void)
{
    char    **cmd;
    int n = 0;

    cmd = malloc(sizeof(char *) * 24);
    while (n < 24)
        cmd[n++] = malloc(100);
    cmd[0] = "ls >kfdjjd >dff";
    cmd[1] = ">sdlkdsk ls >sdkjksd -la";
    cmd[2] = ">    sdsd ls";
    cmd[3] = ">'    sd'sddsdsd ls";
    cmd[4] = ">'   dssds<s' dsdsd ls";
    cmd[5] = "> >dsds ls";
    cmd[6] = "><dssdf ls";
    cmd[7] = ">< dssd ls";
    cmd[8] = ">>> sdsfdf ls";
    cmd[9] = ">>dffd ls";
    cmd[10] = "ls >";
    
    cmd[11] = "dssf< ls";
    cmd[12] = "< ls";
    cmd[13] = "sds<< ls";
    cmd[14] = "<sdsd ls";
    cmd[15] = "<>sdsd ls";
    cmd[16] = "< <sdsd ls";
    cmd[17] = "<<<sdsd ls";
    cmd[18] = "< >sdds ls";
    
    cmd[19] = "'<sdsd' ls";
    cmd[20] = "ls '>sdsd'";
    cmd[21] = "<'<sd'sd ls";
    cmd[22] = "<'dssd' ls";
    cmd[23] = NULL;
    int i = 0;
    char **ret;
    int j;
    while (cmd[i])
    {
        ret = handle_redirect(cmd[i]);
        j = -1;
        printf("%s\n GET:\n", cmd[i]);
        while (ret[++j])
            printf("%s\n", ret[j]);
        i++;
    }
    return (0);
}