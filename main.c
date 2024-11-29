#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct Variable {
    char *variable_name;
    char *variable_data;
    int variable_data_int;
    char *variable_type;
} Variable;

typedef struct IfStatement {
    char *var_to_check_1;
    char *var_to_check_2;
    char *conditional_operator;

    char if_or_elseif;

    char *main_if_block;
    char *else_if_block;
} IfStatement;

int current_line_number = 1;
char Statements[][20] = {
    "OUTPUT",
    "INPUT",
    "DECLARE",
    "IF",
    "ENDIF",
    "FOR",
    "NEXT",
    "WHILE",
    "ENDWHILE",
};

int IsStatement(char *string) {
    for (int i = 0; i < 9; i++) {
        
        if (strcmp(string, Statements[i]) == 0) {
            return 1;
        } else {return 0;}
        
    }
    
}

char *ReplaceCharacters(char* string, char charcter_to_replace, char replacement_char) {
    char *duplicate = malloc(sizeof(char) * strlen(string));
    strcpy(duplicate, string);

    for (size_t i = 0; i < strlen(string); i++) {
        char *character = &duplicate[i];

        if (*character == charcter_to_replace) {
            *character = replacement_char;
        }
        
    }
    
    return duplicate;
}

char *FileToString(char *filename, int *length) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) return NULL;

    // get the no. of chars in file
    fseek(file, 0, SEEK_END);
    int chars_in_file = ftell(file);
    fseek(file, 0, SEEK_SET);

    // create the string to hold the contents
    char *contents = malloc(sizeof(char) * chars_in_file + 1);

    char c;
    int i = 0;

    // read all the data and store into contents
    while ((c = fgetc(file)) != EOF) {
        contents[i] = c;

        i++;
    }

    // add a terminating character at last index
    contents[i] = '\0';

    fclose(file);
    
    *length = chars_in_file;
    return contents;
}


char **Slice(char *file_contents, char *delimiter, int *count, int *items) {
    int len = strlen(file_contents);

    *count = 0;
    int i = 0;

    while (i < len) {
        while (i < len) {
            if (strchr(delimiter, file_contents[i]) == NULL) {
                break;
            }
            i++;
            
        }
        
        int oldI = i;
        while (i < len) {
            if (strchr(delimiter, file_contents[i]) != NULL) {
                break;
            }
            i++;
            
        }

        if (i > oldI) {
            *count++;
        }
        
    }

    char **splitted = malloc(sizeof(char *) * *count);
    
    i = 0;
    char buffer[2048];
    int string_idx = 0;

    while (i < len) {
        while (i < len) {
            if (strchr(delimiter, file_contents[i]) == NULL) {
                break;
            }
            i++;
            
        }
        
        int j = 0;
        while (i < len) {
            if (strchr(delimiter, file_contents[i]) != NULL) {
                break;
            }

            buffer[j] = file_contents[i];
            i++;
            j++;
        }

        if (j > 0) {    
            buffer[j] = '\0';

            int mem_to_alloc = sizeof(char) * strlen(buffer) + 1;

            splitted[string_idx] = malloc(mem_to_alloc);

            strcpy(splitted[string_idx], buffer);

            string_idx++;
        }
    }

    *items = string_idx;

    return splitted;
}


char *ConcatenateStrings(char **strings_array, int no_of_items_in_array, int idx_to_start_conc, int idx_to_end_conc) {
    int total_string_len = 0;
    int total_spaces = 0;
    
    for (int i = idx_to_start_conc; i < no_of_items_in_array; i++) {
        total_string_len += strlen(strings_array[i]);
        total_spaces += 1;
    }
    
    total_spaces++;

    char *string_concatenated = malloc(sizeof(char) * (total_string_len + total_spaces + 1));

    char *current_position = string_concatenated;

    // Concatenate strings one by one
    for (int i = idx_to_start_conc; i < idx_to_end_conc; i++) {
        int len = strlen(strings_array[i]);
        memcpy(current_position, strings_array[i], len);  // Copy the current string
        current_position += len;  // Move the pointer forward by the length of the current string

        if (i < idx_to_end_conc - 1) {
            *current_position = ' ';  // Add a space after each string except the last one
            current_position++;
        }
    }

    *current_position = '\0';  // Null-terminate the string

    return string_concatenated;
}

void RemoveFirstAndLast(char *str) {
    int length = strlen(str);

    if (length > 2) {
        // Shift characters to the left by 1 to remove the first character
        for (int i = 1; i < length - 1; i++) {
            str[i - 1] = str[i];
        }
        
        // Null terminate the new string to remove the last character
        str[length - 2] = '\0';
    }
}

int CountCharacters(char *string, char character) {
    int count = 0;

    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == character) {
            count++;
        }
    }
    
    return count;
}

int ValidateInt(char *string) {
    int len = strlen(string);
    string[len+1] = '\0';

    for (int i = 0; i < len; i++) {
        
        if (!isdigit(string[i])) {

            return -69;
        }
    }

    int int_to_return = atoi(string);

    return int_to_return;
}

char *ValidateString(char *string) {
    int no_of_double_quotes = CountCharacters(string, '"');
    if (string[0] == '"' && string[strlen(string)-1] == '"' && no_of_double_quotes == 2) {
        return string;
    } else {return "NOT_A_STRING_404";}
}

char *ValidateBool(char *string) {
    if (strcmp(string, "true") == 0 || strcmp(string, "false") == 0) {
        return string;
    } else {return "NOT_A_BOOL_404";};
    
}

int IsVar(char *possible_var_name) {
    if (possible_var_name[0] != '"' && !(strcmp(possible_var_name, "true") == 0 || strcmp(possible_var_name, "false") == 0) && ValidateInt(possible_var_name) == -69 && !isdigit(possible_var_name[0])) {
        return 1;
    } else {return 0;}
    
}

Variable ReturnVariable(char **line_spliited, int no_of_items) {
    Variable var;

    if (strcmp(line_spliited[0], "DECLARE") == 0) {
        var.variable_name = strdup(line_spliited[1]);

        if (IsVar(var.variable_name) == 1) {
            int count_doublequotes = CountCharacters(ConcatenateStrings(line_spliited, no_of_items, 3, no_of_items), '"');

            if (no_of_items >= 4) {
                if (strcmp(line_spliited[2], "<-") == 0) {
                    if (count_doublequotes == 2) {
                        var.variable_data = ConcatenateStrings(line_spliited, no_of_items, 3, no_of_items);
                        RemoveFirstAndLast(var.variable_data);
                        if (CountCharacters(var.variable_data, '"') != 0) {
                            printf("Error in value of variable %s on line %d\n", var.variable_name, current_line_number);
                            exit(1);
                        }
                        
                        var.variable_type = "STRING";
                        var.variable_data_int = 0;
                    } else
                    if (line_spliited[3][0] == '\'' && strlen(line_spliited[3]) == 3) {
                        var.variable_data = line_spliited[3];
                        RemoveFirstAndLast(var.variable_data);

                        var.variable_type = "CHARACTER";
                        var.variable_data_int = 0;
                    } else
                    if (strcmp(line_spliited[3], "false") == 0 || strcmp(line_spliited[3], "true") == 0) {
                        var.variable_data = line_spliited[3];
                        var.variable_type = "BOOLEAN";
                        var.variable_data_int = 0;
                    } else
                    if (ValidateInt(line_spliited[3]) != -69) {
                        var.variable_data = "";
                        var.variable_data_int = ValidateInt(line_spliited[3]);
                        var.variable_type = "INTEGER";
                    } else {printf("Error. Variable %s is not assigned a valid data type on line %d\n", line_spliited[1], current_line_number); exit(1);}
                } else {printf("Perhaps you forgot a <- for variable %s on line %d?\n", var.variable_name, current_line_number); exit(1);}
            } else
            if (no_of_items == 2) {
                if (IsVar(line_spliited[1]) == 1) {
                    var.variable_data = "";
                    var.variable_data_int = 0;
                    var.variable_type = "";
                } else {printf("Unknown declaration error for variable %s on line %d\n", var.variable_name, current_line_number); exit(1);}
                
            } else {printf("Invalid declaration for variable %s on line %d\n", var.variable_name, current_line_number); exit(1);}
        } else {
            printf("%s's variable name on line %d is not valid. Make sure variable names dont start with an integer\n", var.variable_name, current_line_number);
            exit(1);
        }
    } else {
        var.variable_name = strdup(line_spliited[0]);

        if (IsVar(var.variable_name) == 1) {
            int count_doublequotes = CountCharacters(ConcatenateStrings(line_spliited, no_of_items, 2, no_of_items), '"');

            if (no_of_items >= 3) {
                if (strcmp(line_spliited[1], "<-") == 0) {
                    if (count_doublequotes == 2) {
                        var.variable_data = ConcatenateStrings(line_spliited, no_of_items, 2, no_of_items);
                        RemoveFirstAndLast(var.variable_data);
                        if (CountCharacters(var.variable_data, '"') != 0) {
                            printf("Error in value of variable %s on line %d\n", var.variable_name, current_line_number);
                            exit(1);
                        }
                        
                        var.variable_type = "STRING";
                        var.variable_data_int = 0;
                    } else
                    if (line_spliited[2][0] == '\'' && strlen(line_spliited[3]) == 3) {
                        var.variable_data = line_spliited[3];
                        RemoveFirstAndLast(var.variable_data);

                        var.variable_type = "CHARACTER";
                        var.variable_data_int = 0;
                    } else
                    if (strcmp(line_spliited[2], "false") || strcmp(line_spliited[2], "true")) {
                        var.variable_data = line_spliited[2];
                        var.variable_type = "BOOLEAN";
                        var.variable_data_int = 0;
                    } else
                    if (ValidateInt(line_spliited[2]) != -69) {
                        var.variable_data = "";
                        var.variable_data_int = ValidateInt(line_spliited[2]);
                        var.variable_type = "INTEGER";
                    } else {printf("Error. Variable %s is not assigned a valid data type on line %d\n", line_spliited[0], current_line_number); exit(1);}
                } else {printf("Perhaps you forgot a <- for variable %s on line %d?\n", var.variable_name, current_line_number); exit(1);}
            } else {printf("Invalid declaration for variable %s on line %d\n", var.variable_name, current_line_number); exit(1);}
        } else {
            printf("%s's variable name on line %d is not valid. Make sure variable names dont start with an integer\n", var.variable_name, current_line_number);
            exit(1);
        }
    }
    
    return var;
}

Variable FindVariable(Variable *VariablesArray, int var_amount, char *var_to_search) {
    int is_var_found = 0;

    for (int i = 0; i < var_amount; i++) {
        if (strcmp(VariablesArray[i].variable_name, var_to_search) == 0) {
            is_var_found = 1;
            return VariablesArray[i];
        }
        
    }
    
    if (is_var_found == 0) {
        Variable error_var;
        error_var.variable_name = "VAR_NOT_FOUND_ERROR_404";
        printf("Variable %s was not found on line %d. Make sure you have declared it before hand\n", var_to_search, current_line_number);
        return error_var;
    }
}

int CheckType(char *data_str, int data_int, char *entered_type, char *type_to_check_against) {
    if (type_to_check_against == "STRING") {
        
    }
    
}

int IsConditionTrue(char *value_1, char *value_2, char *operator) {
    if (strcmp(operator, "=")) {
        if (value_1 == value_2) {
            return 1;
        } else {return 0;}
    } else
    if (strcmp(operator, ">")) {
        if (value_1 > value_2) {
            return 1;
        } else {return 0;}
    } else
    if (strcmp(operator, ">=")) {
        if (value_1 >= value_2) {
            return 1;
        } else {return 0;}
    } else
    if (strcmp(operator, "<")) {
        if (value_1 < value_2) {
            return 1;
        } else {return 0;}
    } else
    if (strcmp(operator, "<=")) {
        if (value_1 <= value_2) {
            return 1;
        } else {return 0;}
    } else
    if (strcmp(operator, "<>")) {
        if (value_1 != value_2) {
            return 1;
        } else {return 0;}
    } else {
        return 10;
    }
}

void Output(char *string_to_output) {
    printf("%s\n", string_to_output);
}

char *slice(const char* str, size_t start, size_t end) {
    char *result = malloc(sizeof(char) * strlen(str));
    strncpy(result, str + start, end - start);

    return result;
}

void UpdateVariableInList(Variable *variables_array, int var_amount, char *name_of_variable_to_update, Variable updated_variable) {

    for (int i = 0; i < var_amount; i++) {
        if (strcmp(variables_array[i].variable_name, name_of_variable_to_update) == 0) {
            
            variables_array[i] = updated_variable;
            
            return;
        }
        
    }
    
}



int main(int argc, char **argv) {
    // disable buffering for output
    setbuf(stdout, NULL);

    //printf("%s\n", argv[1] + strlen(argv[1])-3);
    // checking if input file is a valid .ps
    if (strcmp((argv[1] + strlen(argv[1])-3),".ps") != 0) {
        printf("Please provide a valid pseudocode file\n");
        
        return 1;
    }
    

    // convert file to string
    int len_of_file = 0;
    char *file_contents = FileToString(argv[1], &len_of_file);

    if (file_contents == NULL) {
        printf("Error reading the file. Please make sure you are not supplying an empty file");
        return 1;
    }

    // slice file with \n as delimeter
    int c = 0;
    int lines = 0;
    char **sliced = Slice(file_contents, "\n", &c, &lines);

    // free the stringed file since no need
    free(file_contents);

    // holding all the variables
    Variable VARIABLES[sizeof(Variable) * lines];
    int variable_amount = 0;

    for (int i = 0; i < lines; i++) {
        int line_c;
        int line_items;

        current_line_number = i+1;

        char **line = Slice(sliced[i], " ", &line_c, &line_items);

        if (strcmp(line[0], "DECLARE") == 0) {
            // adding variables to the VARIABLES array
            Variable v = ReturnVariable(line, line_items);
            VARIABLES[variable_amount] = v;
            variable_amount++;
        } else
        if (strcmp(line[0], "INPUT") == 0) {
            char input_buffer[1024];

            if (IsVar(line[1]) == 1) {
                Variable input_var = FindVariable(VARIABLES, variable_amount, line[1]);
                if (strcmp(input_var.variable_name, "VAR_NOT_FOUND_ERROR_404") == 1) {
                    fgets(input_buffer, sizeof(input_buffer), stdin);
                    
                    input_var.variable_data = input_buffer;
                    input_var.variable_type = "STRING";

                    UpdateVariableInList(VARIABLES, variable_amount, line[1], input_var);
                } else {printf("Assigned variable %s to INPUT on line %d was not found\n", input_var.variable_name, current_line_number); return 1;}
            } else
            if (ValidateString(ConcatenateStrings(line, line_items, 1, line_items-1)) != "NOT_A_STRING_404") {

                if (IsVar(line[line_items-1]) == 1) {
                    Variable input_var = FindVariable(VARIABLES, variable_amount, line[line_items-1]);
                    

                    char *string_to_output_for_input = ConcatenateStrings(line, line_items, 1, line_items-1);
                    RemoveFirstAndLast(string_to_output_for_input);

                    printf("%s\n", string_to_output_for_input);
                    fgets(input_buffer, sizeof(input_buffer), stdin);

                    input_var.variable_data = input_buffer;
                    input_var.variable_type = "STRING";

                    
                    UpdateVariableInList(VARIABLES, variable_amount, input_var.variable_name, input_var);
                }
            }
            
        } else
        if (IsVar(line[0]) == 1 && IsStatement(line[0]) == 0) {

            if (strcmp(line[1], "<-") == 0) {
                Variable to_change = FindVariable(VARIABLES, variable_amount, line[0]);
                
                if (strcmp(to_change.variable_name, "VAR_NOT_FOUND_ERROR_404") == 1) {
                    Variable changed = ReturnVariable(line, line_items);

                    UpdateVariableInList(VARIABLES, variable_amount, changed.variable_name, changed);
                }
                
            }
            
        } else
        if (strcmp(line[0], "OUTPUT") == 0) {
            // outputting either the variable value or the supplied data

            char *to_output = ConcatenateStrings(line, line_items, 1, line_items);
            int is_to_output_a_var = IsVar(to_output);
            if (is_to_output_a_var == 1) {
                Variable to_output_var = FindVariable(VARIABLES, variable_amount, to_output);

                if (to_output_var.variable_data == "") {
                    printf("%d\n", to_output_var.variable_data_int);
                } else {
                    printf("%s\n", to_output_var.variable_data);
                }
                
            } else
            if (ValidateInt(to_output) != -69) {
                printf("%s\n", to_output);
            } else
            if (ValidateBool(to_output) != "NOT_A_BOOL_404") {
                printf("%s\n", to_output);
            } else 
            if (ValidateString(to_output) != "NOT_A_STRING_404") {
                RemoveFirstAndLast(to_output);
                printf("%s\n", to_output);  
            } else {printf("Error. %s is not a valid data type or a declared variable on line %d\n", to_output, current_line_number); return 1;}
        
        } else {printf("Invalid pseudocode statement '%s' on line %d\n", line[0], current_line_number); return 1;}
        
        
        
    }

    return 0;
}