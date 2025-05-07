# Harrison Latimer

import csv
        

def viterbi(text_file, possible_input_tags_file = None):
    """
    Inputs:
    
    
    Outputs:
    
    """
    
    ##########################################################################################
    # Make full transition matrix
    with open('transition_matrix.csv', newline = '') as csvfile:
        transition_matrix_reader = csv.reader(csvfile)
        first = True
        for row in transition_matrix_reader:
            if first:
                pos_array = row[1 :] # Array containing all POS names
            first = False
            
            
    pos_count = len(pos_array) # Number of POS
    pos_dict = {} # Dictionary that maps POS name to index
    i = 0
    for pos in pos_array:
        pos_dict[pos] = i
        i += 1
        
    transition_matrix = [[0.0 for j in range(pos_count)] for i in range(pos_count)]
    with open('transition_matrix.csv', newline = '') as csvfile:
        transition_matrix_reader = csv.reader(csvfile)    
        
        row_index = -1
        for row in transition_matrix_reader:
            col_index = -1
            for col in row:
                if (row_index >= 0) and (col_index >= 0):
                    transition_matrix[row_index][col_index] = float(row[col_index + 1])
                col_index += 1
            row_index += 1
    
    ##########################################################################################
    # Make full emission matrix
    
    with open('emission_matrix.csv', newline = '') as csvfile:
        emission_matrix_reader = csv.reader(csvfile)
        my_words = []
        not_first = False
        for row in emission_matrix_reader:
            # print(row)
            if not_first:
                new_word = row[1]
                if new_word not in my_words:
                    my_words.append(new_word)
            not_first = True
        
    
    my_words_count = len(my_words)
    my_words_dict = {} # Dictionary that maps words to index
    i = 0
    for word in my_words:
        my_words_dict[word] = i
        i += 1
        
    emission_matrix = [[0.0 for j in range(my_words_count)] for i in range(pos_count)]
    with open('emission_matrix.csv', newline = '') as csvfile:
        emission_matrix_reader = csv.reader(csvfile)
        not_first = False
        for row in emission_matrix_reader:
            if not_first:
                pos = row[0]
                if pos == '--s--':
                    pos = 'START'
                word = row[1]
                row_index = pos_array.index(pos)
                col_index = my_words.index(word)
                emission_matrix[row_index][col_index] = float(row[2])
            not_first = True
    
    ##########################################################################################
    # If given a file containing possible POS, adapt transmission and emission matrices
    
    if possible_input_tags_file:
        
        # Read off POS subset from file
        with open(possible_input_tags_file, 'r') as file: # read in the input file
            input_tags_file = file.read()
        new_pos_array = []
        apos = ''
        for char in input_tags_file:
            if char != ' ':
                apos += char
            else:
                new_pos_array.append(apos)
                apos = ''
        new_pos_array.append(apos)
        new_pos_array.append('START')
        new_pos_count = len(new_pos_array)
        new_pos_dict = {} # Dictionary that maps POS name to index
        i = 0
        for pos in new_pos_array:
            new_pos_dict[pos] = i
            i += 1
        
        # Create new transition matrix from old
        new_transition_matrix = [[0.0 for j in range(new_pos_count)] for i in range(new_pos_count)]
        for i in range(new_pos_count):
            for j in range(new_pos_count):
                new_transition_matrix[i][j] = transition_matrix[ pos_dict[new_pos_array[i]] ][ pos_dict[new_pos_array[j]] ]
                
        new_emission_matrix = [[0.0 for j in range(my_words_count)] for i in range(new_pos_count)]
        for i in range(new_pos_count):
            for j in range(my_words_count):
                new_emission_matrix[i][j] = emission_matrix[ pos_dict[new_pos_array[i]] ][ j ]
                
        pos_array = new_pos_array
        pos_count = new_pos_count
        pos_dict = new_pos_dict
        transition_matrix = new_transition_matrix
        emission_matrix = new_emission_matrix
        
    
    ##########################################################################################
    # Read in text file
    with open(text_file, 'r') as file: # read in the input file
        input_file = file.read()
       
    text_as_words = []
    aword = ''
    for char in input_file:
        if char != ' ':
            aword += char
        else:
            text_as_words.append(aword)
            aword = ''
    text_as_words.append(aword)
    
    ##########################################################################################
    # Perform the Viterbi algorithm
    
    T = len(text_as_words)
    delta = [[0.0 for j in range(T)] for i in range(pos_count)]
    psi = [[0 for j in range(T)] for i in range(pos_count)]
    
    # Initialize maxprobs delta and traceback psi
    pos_index = pos_dict['START']
    word_index = my_words_dict[text_as_words[0]]
    for i in range(pos_count):
        delta[i][0] = transition_matrix[pos_index][i] * emission_matrix[i][word_index]
        psi[i][0] = pos_index
    
    # Code for recursion
    for t in range(1, T):
        obs_word_index = my_words_dict[text_as_words[t]]
        for s in range(pos_count):
            the_max = 0.0
            for s_prime in range(pos_count):
                new_val = delta[s_prime][t - 1] * transition_matrix[s_prime][s] * emission_matrix[s][obs_word_index]
                if new_val > the_max:
                    delta[s][t] = new_val
                    the_max = new_val
                    psi[s][t] = s_prime
    
    # Code for traceback
    the_max = 0.0
    s = 0
    for i in range(pos_count):
        if delta[i][T - 1] > the_max:
            the_max = delta[i][T - 1]
            s = i
    most_likely_pos_tags_indices = [0 for i in range(T)]
    most_likely_pos_tags_indices[T - 1] = s
    for i in range(T - 1):
        most_likely_pos_tags_indices[T - i - 2] = psi[most_likely_pos_tags_indices[T - i - 1]][T - i - 1]
    most_likely_pos_tags = ['' for i in range(T)]
    for i in range(T):
        most_likely_pos_tags[i] = pos_array[most_likely_pos_tags_indices[i]]
#     print('Most likely pos tags:', most_likely_pos_tags)
#     print(text_as_words)

    ##########################################################################################
    # Write to output file
    
    text_number = text_file[6 : -4]
    output_filename = 'output_' + str(text_number) + '.txt'
    
    with open(output_filename, 'w') as file:
        for i in range(T - 1):
            file.write(most_likely_pos_tags[i])
            file.write(' ')
        file.write(most_likely_pos_tags[T - 1])
        
    

    
    
    
    
    

    
    
