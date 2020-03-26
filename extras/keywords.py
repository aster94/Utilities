#! python3
"""
Automatically generate the keywords.txt from a header file or make an existing
keywords.txt with a uniform indentation

@author: aster94
"""
# Settings
newline = '\n' # compatibility between unix, windows, mac?
distance = 8

template_KEYWORD1 = """\
#######################################
# Syntax Coloring Map
#######################################

#######################################
# Datatypes (KEYWORD1)
#######################################
"""

template_KEYWORD2 = """\
#######################################
# Methods and Functions (KEYWORD2)
#######################################
"""

template_LITERAL1 = """\
######################################
# Constants (LITERAL1)
######################################
"""

# Modules
import click, re, os, shutil, sys

############################################################
######                   Utilities                   #######
############################################################
def file_exist(file_path):
    return os.path.isfile(file_path)

def file_read(file_path):
    with open(file_path, 'r') as file:
        return file.read()

def file_write(file_path, content):
    with open(file_path, 'w') as file:
        file.write(content)

def file_copy(source, destination):
    shutil.copy(source, destination)

def file_create(file_path):
    with open(file_path, 'w+'): pass


############################################################
######                   Functions                   #######
############################################################
def read_header(file_path, verbose, force):

    block_comment = False
    polished = ""
    key = {'KEYWORD1': list(), 'KEYWORD2': list(), 'LITERAL1': list()}

    raw = file_read (file_path)

    # Integrity checks
    multiline_comment_start = raw.count('/*')
    multiline_comment_end = raw.count('*/')
    if (multiline_comment_start == 0 and multiline_comment_end == 0):
        pass
    elif (multiline_comment_start - multiline_comment_end != 0):
        print('check source file: multiline comment block problem')
        if not force: raise NameError('MultilineBlockUndefinied')

    # Clean
    for line in raw.splitlines():
        line = line.strip()
        if (block_comment):
            # We are inside a multiline block comment
            if (line.find('*/') == -1):
                continue
            else:
                block_comment = False
                line = line[line.find('*/')+2:]

        if (line.find('/*') != -1):
            # It is the start of a block comment
            block_comment = True
            line = line[:line.find('/*')]

        if (line.find('//') != -1):
            line = line[:line.find('//')]

        if (line.find('#') != -1):
            continue

        polished += line + '\n'

    # Removing empty lines
    polished = re.sub(r'\n\s*\n', '\n', polished.strip())

    if ('public:' in polished):
        start = polished.find('public:') + 8
    else:
        start = polished.find('{') + 2

    if ('private:' in polished):
        end = polished.find('private:')
    elif ('protected:' in polished):
        end = polished.find('protected:')
    else:
        end = polished.find('}')

    if (start > end):
        print('Unexpected error')
        raise NameError('Unexpected error')

    function_block = polished[start:end].strip()

    for line in function_block.splitlines():
        line = line.strip()

        start = line.find(' ') + 1
        end = line.find('(')

        # Add them to list
        if (start > end):
            to_add = line[:end]
            if (not to_add in key['KEYWORD1']): key['KEYWORD1'].append(to_add)
        else:
            to_add = line[start:end]
            if (not to_add in key['KEYWORD2']): key['KEYWORD2'].append(to_add)

    return key

def read_keywords(file_path, verbose, force):

    key = {'KEYWORD1': list(), 'KEYWORD2': list(), 'LITERAL1': list()}
    raw = file_read(file_path)
    value = ''

    for line in raw.splitlines():
        line = line.strip()
        if ('# ' in line):
            value = line[line.find('(')+1:line.find(')')]
        elif ('#' in line or not line):
            continue
        else:
            try:
                last = re.search('\s', line).start()
            except:
                last = len(line)
            key[value].append(line[:last])
    return key

def write_keywords(key_dict, file_path, verbose, soft):
    max_len = 0
    '''
    # Get the longest item
    for k in key_dict.keys():
        for i in key_dict[k]:
            i_len = len (i)
            if (max_len < i_len): max_len = i_len

    # Extend it
    while (max_len % 4 != 0): max_len += 1
    max_len += distance
    '''
    output = template_KEYWORD1
    for n in key_dict['KEYWORD1']:
        #pos = max_len - len(n)
        output += '{}\t{}\n'.format(n, 'KEYWORD1')

    output += '\n\n' + template_KEYWORD2
    for n in key_dict['KEYWORD2']:
        #pos = max_len - len(n)
        output += '{}\t{}\n'.format(n, 'KEYWORD2')

    output += '\n\n' + template_LITERAL1
    for n in key_dict['LITERAL1']:
        #pos = max_len - len(n)
        output += '{}\t{}\n'.format(n, 'LITERAL1')

    if verbose: print('{} printed/wrote'.format(file_path))
    if soft: print(output.strip())
    else: file_write(file_path, output.strip())


@click.command()
@click.argument('file_path')
@click.option('--backup' , '-b', is_flag=True, help='Create a backup with the original keywords.txt file.')
@click.option('--verbose' , '-v', is_flag=True, help='Verbose output.')
@click.option('--force' , '-f', is_flag=True, help='Overwrite existing files.')
@click.option('--soft' , '-s', is_flag=True, help='Don\'t write files, print them in the console.')
@click.option('--preserve' , '-p', is_flag=True, default=True, help='Don\'t preserve LITERAL1 even if keywords.txt already exists.')
def keywords(file_path, backup, verbose, force, soft, preserve):
    """
    Automatically generate the keywords.txt from a header file or make an existing
    keywords.txt with a uniform indentation
    """

    if verbose: print('Arguments\nfile_path: {}\nbackup: {}\nverbose: {}\
                      \nforce: {}\nsoft: {}\npreserve: {}\n'.format(\
                      file_path, backup, verbose, force, soft, preserve))

    if not file_exist(file_path):
        print('File not found')
        raise NameError('FileNotExists')

    key_dict = dict()
    extension = os.path.splitext(file_path)[1]

    if (extension == '.h'):
        key_dict = read_header(file_path, verbose, force)
        keywords_path = os.path.dirname(file_path)+'\keywords.txt'
        if preserve:
            if file_exist(keywords_path):
                if verbose: print('Copying LITERAL1 from {}'.format(keywords_path))
                key_dict_old = read_keywords(keywords_path, verbose, force)
                key_dict['LITERAL1'] = key_dict_old['LITERAL1']
            else:
                if verbose: print('Can\'t copy LITERAL1, no keywords.txt found')
    elif (extension == '.txt'):
        key_dict = read_keywords(file_path, verbose, force)
        keywords_path = file_path
    else:
        print('File format not accepted')
        raise NameError('FileNotAccepted')

    # Check if keywords.txt exist
    if(file_exist(keywords_path)):
        if force:
            if verbose: print('{} already exist, overwritten'.format(keywords_path))
        else:
            if verbose: print('{} already exist, not overwritten'.format(keywords_path))
            sys.exit(0)
        if (backup):
            backup_path = keywords_path + '.old'
            if file_exist(backup_path):
                if force:
                    if verbose: print('{} already exist, overwritten'.format(backup_path))
                    file_copy(keywords_path, backup_path)
                else:
                    if verbose: print('{} already exist, not overwritten'.format(backup_path))
            else:
                if verbose: print('{} didn\'t exist, created'.format(backup_path))
                file_copy(keywords_path, backup_path)
    else:
        if verbose: print('{} created'.format(keywords_path))
        file_create(keywords_path)

    write_keywords(key_dict, keywords_path, verbose, soft)

def main():
    keywords()

if __name__ == '__main__':
    main()
