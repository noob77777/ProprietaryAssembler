#include "./second_pass_parse.h"

int main(int argc, char **argv) {
    char *filename; FILE *fp;
    char *rest = NULL; char *token = NULL;

    second_pass_token *tokens = NULL; int num_tokens = 0;

    if (!argv[1]) {
        fprintf(stderr, "Usage: ./a.out filename: (char *)\n");
        exit(EXIT_FAILURE);
    } else
        filename = argv[1];

    if((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error while opening file.\n");
        exit(EXIT_FAILURE);
    }

    size_t buf_size = 80;
    char *buffer = malloc(buf_size * sizeof(char));

    while(getline(&buffer, &buf_size, fp) != -1) {
        rest = buffer;

        num_tokens = 0;

        tokens = malloc(num_tokens * sizeof(second_pass_token));

        while(token = strtok_r(rest, " ", &rest)) {
			void *temp = NULL;
			if(temp = realloc(tokens, (++num_tokens * sizeof(second_pass_token)))) {
				tokens = temp;
			} else {
                fprintf(stderr, "realloc error.\n");
                exit(EXIT_FAILURE);
            }

            char *str = malloc(strlen(token) * sizeof(char));
            str = strip_newline(token);

            second_pass_token t = make_token(str);

            tokens[num_tokens - 1] = t;
		}

        line line;
        line.len = num_tokens;
        line.tokens = malloc(line.len * sizeof(second_pass_token));
        for(int i = 0; i < line.len; i++) {
            line.tokens[i] = tokens[i];
        }

        print_line(line);

        free(tokens);
        free(token);
        free(line.tokens);
    }
}