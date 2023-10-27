#!/usr/bin/env python3

import os
import re
from pycparser import c_parser, c_ast, parse_file

def remove_unused_code(file_path):
    with open(file_path, 'r') as file:
        code = file.read()

    parser = c_parser.CParser()
    ast = parse_file(file_path, use_cpp=True)

    # Identify unused functions and variables
    unused_code = []
    for ext in ast.ext:
        if isinstance(ext, c_ast.FuncDef):
            if ext.decl.name not in code:
                unused_code.append(ext.decl.name)
        elif isinstance(ext, c_ast.Decl):
            if ext.name not in code:
                unused_code.append(ext.name)

    # Identify commented out code
    commented_code = re.findall(r'//.*', code)

    # Remove unused code and commented out code
    for item in unused_code + commented_code:
        code = code.replace(item, '')

    # Write the new code to the file
    with open(file_path, 'w') as file:
        file.write(code)

def main():
    for root, dirs, files in os.walk('.'):
        for file in files:
            if file.endswith('.c'):
                remove_unused_code(os.path.join(root, file))

if __name__ == '__main__':
    main()
