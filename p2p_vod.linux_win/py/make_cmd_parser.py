__author__ = 'wbtang'

import log_base

cmd_ini = log_base.file_parser_ini
class_name = 'CommandLineParser'
parser_name = 'Parser'

parser = []
debuger = []
accessors = []
members = []
default_value = []

def compress(data, num_tabs):
    tab = ''
    for i in range(num_tabs):
        tab += '\t'
    all = ''
    for line in data:
        all += '%s%s\n' % (tab, line)
    return all

def all_in_one():
    lines = []
    lines.append("""
//
// Generated on %s
//
// Command Line Parser
//
// THIS IS AN AUTO-GENERATED FILE.
// DO NOT MODIFY IT MANUALLY!!!
//

#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class %s {
public:
//
// PARSER
//
    static void %s (string key, string value) {
        cout << "Parsing key ( " << key << " ) " << "value ( " << value << " )" << endl;
        for ( auto &c : key) {
            c = tolower(c);
        }
        if ( key == "" ) {
            return ;
%s
        } else {
            cout << "UNKNOWN COMMAND" << endl;
            EorrorHandle();
        }
    }

    static void EorrorHandle() {
        cout<< "continue? (y/n)" << endl;
        char buf[100];
        gets(buf);
        if ( strcmp(buf, "y") != 0 ) {
            exit(0);
        }
    }

//
// DEBUGER
//
    static void output (FILE *file = stdout) {
%s
        fprintf(file, "# -------------END OF DEBUGER & AUTO GENERATED SCRIPTS-------------\\n");
    }

//
// ACCESSORS - GET
//
%s

private:
//
// Utility
//
    static bool ToBool (const char *str) {
        return strcmp(str, "true") == 0;
    }

    static string ToString (const char *str) {
        string ret = str;
        if ( ret[0] == '"' ) {
            return ret.substr(1, ret.length()-2);
        } else {
            return ret;
        }
    }

//
// MEMBERS
//
%s
};

//
// DEFAULT VALUE
//
%s

// -------------END OF DEBUGER & AUTO GENERATED SCRIPTS-------------
""" % (log_base.get_time(),
       class_name,
       parser_name,
       compress(parser, 2),
       compress(debuger, 2),
       compress(accessors, 1),
       compress(members, 1),
       compress(default_value, 0) )
)

    return lines

def create_head_file(head_file, lines):
    try:
        with open(head_file, 'w') as header:
            for line in lines:
                print(line, file=header)
    except IOError as err:
        print(err)

def append_comment(comment):
    parser.append(comment)
    debuger.append('fprintf(file, "%s\\n");' % (comment))
    accessors.append(comment)
    members.append(comment)
    default_value.append(comment)

def get_type(value):
    if value == 'true' or value == 'false':
        return 'bool', 'ToBool'
    elif value.isdigit():
        return 'int', 'atol'
    elif value[0] == '"':
        return 'string', 'ToString'
    else:
        return 'double', 'atof'

def append_data(key, value):
    type, caster = get_type(value)

    parser.append('} else if ( key == "%s" ) {' % (key))
    parser.append('\t%s_ = %s(value.c_str());' % (key, caster))

    if type == 'string':
        debuger.append('fprintf(file, "%-10s%-27s = %%s\\n", %s_.c_str());' % (type, key, key))
    else:
        debuger.append('fprintf(file, "%-10s%-27s = %%s\\n", to_string(%s_).c_str());' % (type, key, key))

    accessors.append('inline static %s %s() {' % (type, key))
    accessors.append('\treturn %s_;' % (key))
    accessors.append('}')

    members.append('static %s %s_;' % (type, key))

    default_value.append('%s %s::%s_ = %s;' % (type, class_name, key, value))

for line in open(cmd_ini):
    line = line.strip()
    if line == '' or line[0] == '#':
        pass
    elif line[0] == '/':
        append_comment(line)
    else:
        tokens = line.strip().split(' ', 1)
        if tokens[0] == 'ClassName':
            class_name = tokens[1]
        elif tokens[0] == 'ParserName':
            parser_name = tokens[1]
        else:
            append_data(tokens[0].lower(), tokens[1])

all = all_in_one()
head_file = '%s/%s.h' % (log_base.folder_src, class_name)
create_head_file(head_file, all)
