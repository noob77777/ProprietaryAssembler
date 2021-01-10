#include "main.h"
#include "trie.h"
#include <string.h>





int get_code_for_register( char *reg_name ){

    int len = strlen(reg_name) ;
    int i;
    for ( i= 0; i < len; i++ ){
        reg_name[i] = toUpper(reg_name[i]);
    }

    if ( strcmp(reg_name, "AL") == 0 ){
        return 0;
    }
    if ( strcmp(reg_name, "CL") == 0 ){
        return 1;
    }
    if ( strcmp(reg_name, "DL") == 0 ){
        return 2;
    }
    if ( strcmp(reg_name, "BL") == 0 ){
        return 3;
    }
    if ( strcmp(reg_name, "AH") == 0 ){
        return 4;
    }
    if ( strcmp(reg_name, "CH") == 0 ){
        return 5;
    }
    if ( strcmp(reg_name, "DH") == 0 ){
        return 6;
    }
    if ( strcmp(reg_name, "BH") == 0 ){
        return 7;
    }
    if ( strcmp(reg_name, "AX") == 0 ){
        return 8;
    }
    if ( strcmp(reg_name, "CX") == 0 ){
        return 9;
    }
    if ( strcmp(reg_name, "DX") == 0 ){
        return 10;
    }
    if ( strcmp(reg_name, "BX") == 0 ){
        return 11;
    }
    if ( strcmp(reg_name, "SP") == 0 ){
        return 12;
    }
    if ( strcmp(reg_name, "BP") == 0 ){
        return 13;
    }
    if ( strcmp(reg_name, "SI") == 0 ){
        return 14;
    }
    if ( strcmp(reg_name, "DI") == 0 ){
        return 15;
    }
    if ( strcmp(reg_name, "ES") == 0 ){
        return 16;
    }
    if ( strcmp(reg_name, "CS") == 0 ){
        return 17;
    }
    if ( strcmp(reg_name, "SS") == 0 ){
        return 18;
    }
    if ( strcmp(reg_name, "SS") == 0 ){
        return 19;
    }

    return -1;
}

int symtable_function(char *token, int define)
{
	int pos;
	for(pos = 0; pos < num_symbols; pos++)
        if (strcmp(token, symbol_table[pos].label) == 0)
        	break;
    if(pos == num_symbols)
    {
    	sym_tab_node node;
	    node.defined = 0;
	    node.label = token;
	    node.sl_no = num_symbols;
	    symbol_table[num_symbols++] = node;
	    pos = num_symbols - 1;
    }
    if(define)
    {
    	if(!node.defined)
    	{
    		symbol_table[pos].defined = 1;
        	symbol_table[pos].LC = current_lc;
    	}
    	else
    		return -1;
    }
    return 0;
}

void dfs(struct Trie *tmp, int *current_opcodes, int *size, op_tab_node **opcodes)
{
    if (tmp == NULL)
        return;

    if (tmp->isLeaf)
    {
        opcodes[*current_opcodes] = (tmp->pointer);
        *current_opcodes = *current_opcodes + 1;
        if (*current_opcodes == *size)
        {
            *size = *size + 10;
            opcodes = (op_tab_node **)realloc(opcodes, (*size) * (sizeof( op_tab_node *) ));
        }
    }

    int i;
    for (i = 0; i < CHAR_SIZE; i++)
        dfs(tmp->character[i], current_opcodes, size, opcodes);
}

op_tab_node **search(struct Trie *head, char *opcode, int *current_opcodes)
{

    if (head == NULL)
        return NULL;

    struct Trie *temp = head;

    int size_optabs = 10;
    op_tab_node **opcode_tabs = (op_tab_node **)malloc(size_optabs * sizeof(op_tab_node *));
    

    int len = strlen(opcode);

    int i;
    for (i = 0; i < len; i++)
    {
        if (temp->character[indexTrie(toUpper(opcode[i]))] == NULL)
        {
            printf("Opcode not found");
            return NULL;
        }
        temp = temp->character[indexTrie(opcode[i])];
    }

    dfs(temp, current_opcodes, &size_optabs, opcode_tabs);
    opcode_tabs = (op_tab_node **) realloc(opcode_tabs , (*current_opcodes) * sizeof(op_tab_node *));

    return opcode_tabs;
}

char *strip(char *line)
{
    char *s = strdup(line);
    int len = strlen(s);
    char *back = s + len;
    while (*back == ' ')
        back--;
    *(back + 1) = '\0';
    while (*s == ' ')
        s++;
    return s;
}

int check_number(char *s)
{
    int d = atoi(s);
    // not a number
    if (d == 0)
        return 0;

    /**
     * Accepted numbers of typed 00123 or 1234H
     * 0x1223 not supported 
     */
    int num_digs = 0, num_chars = 0;

    for (; *s; s++)
    {
        if (isdigit(*s))
            num_digs++;
        else
            num_chars++;
    }
    if (num_chars <= 1)
    {
        if (num_chars == 0)
            return 1;
        if (tolower(s[strlen(s) - 1]) == 'h')
            return 1;
    }

    return 0;
}

char * upper_token( char *s ){
    char *ret  = strdup(s);
    
    for ( ; *ret; ret++)
        *ret=toUpper(*ret);
    return ret;
}

int check_reg_var_pair( char * operand, char *token ){

    if (strcmp("REG16(AX)", operand) == 0){
        return strcmp(upper_token(token), "AX") == 0;
    }
    if (strcmp("REG16(BP)", operand) == 0){
        return strcmp(upper_token(token), "BP") == 0;
    }
    if (strcmp("REG16(CX)", operand) == 0){
        return strcmp(upper_token(token), "CX") == 0;
    }
    if (strcmp("REG16(BX)", operand) == 0){
        return strcmp(upper_token(token), "BX") == 0;
    }
    if (strcmp("REG16(DI)", operand) == 0){
        return strcmp(upper_token(token), "DI") == 0;
    }
    if (strcmp("REG16(DX)", operand) == 0){
        return strcmp(upper_token(token), "DX") == 0;
    }
    if (strcmp("REG16(SI)", operand) == 0){
        return strcmp(upper_token(token), "SI") == 0;
    }
    if (strcmp("REG16(SP)", operand) == 0){
        return strcmp(upper_token(token), "SP") == 0;
    }
    if (strcmp("REG8(AH)", operand) == 0){
        return strcmp(upper_token(token), "AH") == 0;
    }
    if (strcmp("REG8(AL)", operand) == 0){
        return strcmp(upper_token(token), "AL") == 0;
    }
    if (strcmp("REG8(BH)", operand) == 0){
        return strcmp(upper_token(token), "BH") == 0;
    }
    if (strcmp("REG8(BL)", operand) == 0){
        return strcmp(upper_token(token), "BL") == 0;
    }
    if (strcmp("REG8(CH)", operand) == 0){
        return strcmp(upper_token(token), "CH") == 0;
    }
    if (strcmp("REG8(CL)", operand) == 0){
        return strcmp(upper_token(token), "CL") == 0;
    }
    if (strcmp("REG8(DH)", operand) == 0){
        return strcmp(upper_token(token), "DH") == 0;
    }
    if (strcmp("REG8(DL)", operand) == 0){
        return strcmp(upper_token(token), "DL") == 0;
    }

    if ( strcmp("AL", operand) == 0){
        return strcmp(upper_token(token) , "AL") == 0;
    }
    if ( strcmp("AH", operand) == 0){
        return strcmp(upper_token(token) , "AH") == 0;
    }
    if ( strcmp("CS", operand) == 0){
        return strcmp(upper_token(token) , "CS") == 0;
    }
    if ( strcmp("DS", operand) == 0){
        return strcmp(upper_token(token) , "DS") == 0;
    }
    if ( strcmp("ES", operand) == 0){
        return strcmp(upper_token(token) , "ES") == 0;
    }
    if ( strcmp("IMM16", operand) == 0 || strcmp(operand, "IMM8") == 0){
        return check_number(token);
    }
    if ( strcmp("MEM16", operand) == 0 || strcmp(operand, "MEM8") == 0){
        return !check_number(token);
    }

    if (strcmp("REG8", operand) == 0 || strcmp("R/M8", operand) == 0)
    {
        int code = get_code_for_register(token);
        if (code >=0 && code < 8 ) return 1;
        if (strcmp("R/M8", operand) == 0) return !check_number(token);
        return 0;

    }
    if (strcmp("REG16", operand) == 0 || strcmp("R/M16", operand) == 0)
    {
        int code = get_code_for_register(token);
        if (code >= 8 && code < 12) return 1;
        if (strcmp("R/M16", operand) == 0)
            return !check_number(token);
        return 0;
    }

    return 1;
}

op_tab_node* validate_and_find( char **tokens , int num_tokens ){

    struct Trie * head;
    op_tab_node* node = NULL;
    if(num_tokens == 0)
        return NULL;


    FILE *fp;
    fp = fopen("intermediate_file.txt", "w");
    int i, j, flag = 1, start_token = 0, temp;
    char *identifier, buffer[4];
    if (tokens[0][strlen(tokens[0]) - 1] == ':')
    {
    	start_token++;
    }
    if(!(strcmp(tokens[start_token], "DB") && strcmp(tokens[start_token], "DW") && strcmp(tokens[start_token], "DQ")))
    {
    	fprintf(fp, "%s", tokens[start_token]);
    	for(i = 0; i < 4 - strlen(tokens[start_token]); i++)
    		fprintf(fp, "%c", '0');
    	fprintf(fp, "%s\n", tokens[start_token + 1]);
    	temp = symtable_function(identifier, 1);
    }
    else
    {

    	char *opcode = tokens[start_token];
	    int num_opcodes = 0;

	    // check in opcode trie
	    op_tab_node ** opcodes = search( head, opcode, &num_opcodes);

	    if(opcodes == NULL || num_opcodes == 0)
	        return NULL;

	    for(i = 0; i < num_opcodes ; i++)
	    {
	        if (strcmp(opcode , opcodes[i]->opcode))
	            continue;

	        char *add_mode = opcodes[i]->add_mode;
	        char *modes[2];
	        int c = 0;

	        char *tmp = strdup(add_mode);
	        char *t = NULL, *tok = tmp;
	        char *s;
	        while((t = strtok(tok, ";")) != NULL)
	        {
	            tok = NULL;
	            s = strip(t);
	            modes[c++] = s;
	        }

	        if(num_tokens - current_token < c)
	            return NULL;

	        for(j = 0; j < c; j++)
	            if(!check_reg_var_pair(modes[j], tokens[current_token+j]))
	                flag=0;

	        if(flag)
	            break;
	    }
	    temp = i;
	    if(temp != num_opcodes)
	    {
	    	fprintf(fp, "O");
	    	for(i = 0; i < 4 - strlen(opcodes[temp]); i++)
    			fprintf(fp, "%c", '0');
    		fprintf(fp, "%s ", opcodes[temp]);

	    	for(i = start_token + 1; i < num_tokens; i++)
	    	{
	    		int reg_code = get_code_for_register(tokens[i]);
	    		if(reg_code == -1)
	    		{
	    			temp = symtable_function(tokens[i], 0);
	    			fprintf(fp, "%s%04d", "S", num_symbols);
	    		}
	    		else
	    		{
	    			fprintf(fp, "%s%04d", "R", reg_code);
	    		}
	    		if(i != num_tokens - 1)
	    			fprintf(fp, " ");
	    		else
	    			fprintf(fp, "\n");
	    	}
	    }
    }
    fclose(fp);
}