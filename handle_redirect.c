#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>

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
    int j;
    char    *ret;

    j = *k;
    i = rotate_quotes_and_text(s, j);
    ret = malloc(sizeof(char) * (i - j + 1));
    if (ret == NULL)
        exit (1); //TODO free all
    i = -1;
    while (s[j] != ' ' && s[j] != '<' && s[j] != '>' && (s[j]))
    {
        if (s[j] == 34 && ft_strchr(&s[j], 34) != NULL)
        {
            while (s[++j] != 34)
                ret[++i] = s[j];
            j++;
            continue;
        }
        if (s[j] == 39 && ft_strchr(&s[j], 39) != NULL)
        {
            while (s[++j] != 39)
                ret[++i] = s[j];
            j++;
            continue;
        }
        if (s[j] != ' ' && s[j] != '>' && s[j] != '<')
        {
          //  printf("%d %d\n", i, j);
            ret[++i] = s[j++];
        }
    }
    while (s[j] == ' ')
        j++;
    //printf("here j = %d s[j] - %c\n", j, s[j]);
    ret[++i] = 0;
    *k = j;
    //printf("ret - |%s|\n", ret);
    return (ret);
}

static int  redirect_create_outputs(char *s, int k)
{
    int fd;


    if (s[k] == '<')
    {
        if (s[k + 1] == '<')
        {
            k++;
            while (s[++k] == ' ')
            {}
            if (s[k] == '>')
                return (-1); //exit (1);//free all syntax error
            else if (s[k] == '<')
                return (-1); //exit (1);//free all syntax error
            fd = open(get_arg(s, &k), O_RDONLY); // some mode
        }
        else if (s[k + 1] == ' ')
        {
            while (s[++k] == ' ')
            {}
            if (s[k] == '<')
                return (-1); //exit (1);//free all syntax error       
            else if (s[k] == '>')
                return (-1); //exit (1); //free all syntax error
            else
                fd = open(get_arg(s, &k), O_RDONLY);
        }
        else
            fd = open(get_arg(s, &k), O_RDONLY);
            //if (dup2(fd, 0) < 0)
          //  {}// free all without exit
    }
    if (s[k] == '<' && s[k + 1] == '>')
        fd = open(get_arg(s, &k), O_TRUNC | O_CREAT | O_RDWR, 0664);
    if (s[k] == '>')
    {
        if (s[++k] == '<')
        {
            return (-1); //TODO add cases and fix
            if (s[k + 2] == '>')
                return (-1); //exit (1);//free all syntax error
            else if (s[k + 2] == '<')
                return (-1); //exit (1);//free all syntax error
            //get filename
            // some mode free all syntax error
        }
        else if (s[k] == 0)
            return (-1); //exit (1); //syntax error
        else if (s[k] == '>')
            fd = open(get_arg(s, &k), O_TRUNC | O_CREAT | O_RDWR | O_APPEND, 0664);
        else if (s[k] == ' ')
        {
            while (s[k] == ' ')
                k++;
            if (s[k] == '<')
                return (-1); //exit (1);//free all syntax error
            else if (s[k] == '>')
                return (-1); //exit (1);//free all syntax error
            else
            {
                char *cmd = get_arg(s, &k);
                fd = open(cmd, O_TRUNC | O_CREAT | O_RDWR, 0664);
            }
        }
        else
        {
            char *cmd = get_arg(s, &k);
            fd = open(cmd, O_TRUNC | O_CREAT | O_RDWR, 0664);
        }
     //   if (dup2(fd, 1) < 0)
       //     {}//free all 
    }
    if (fd < 0)
        return (-1);//exit (1);//free all
    return (k);
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
    //printf("%d %s\n", k, s);
    k = 0;
    j = -1;
    while (s[k])
    {
        //printf("%s\n", &s[k]);
        if (s[k] == '>' || s[k] == '<')
        {
          //  printf("here3 %s  %s\n", s, &s[k]);
            k = redirect_create_outputs(s, k);
            //printf("k = %d\n", k);
            if (k == -1)
                return (NULL);
        }
        else
        {
          //  printf ("here\n");
            ret[++j] = get_arg(s, &k);
         // printf("get arg - |%s| j - |%d|\n", ret[j], j);
        }
    }
    ret[++j] = NULL;
    return (ret);
}

int main(void)
{
    char    **cmd;
    int n = 0;

    cmd = malloc(sizeof(char *) * 24);
    while (n < 24)
        cmd[n++] = malloc(100);
    cmd[0] = "ls -la";
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
        printf("\n%s\n", cmd[i]);
        if (ret != NULL)
            while (ret[++j])
                printf("get: |%s|\n", ret[j]);
        i++;
    }
    return (0);
}