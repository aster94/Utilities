#! python3
"""
Wrap a word around any symbol for documentation of source code

@author: aster94
"""
import pyperclip, click

@click.command()
@click.argument('word')
@click.option('--wrapper' , '-w', default='/', help='Choose the symbol for the wrap')
def wrap(word, wrapper): 
    """
    Wrap a word around any symbol for documentation of source code
    """
    total_lenght = 60
    side_hashtag = 6
    spaces = int((total_lenght - side_hashtag * 2 - len(word)) / 2)
    if (len(word) % 2 == 1): extra = ' '
    else: extra = ''
    
    string = '{0}\n{1}{2}{3}{2}{4}{1}\n{0}\n'.format(wrapper * total_lenght, wrapper * side_hashtag, ' ' * spaces, word, extra)
    
    pyperclip.copy(string)
    print(string)
    print('Text already copied in the clipboad!')

if __name__ == '__main__':
    wrap()