/*!
 * \file main.c
 * \brief Краткое описание
 *
 * Полное описание
 *
 * $Id: $
 *
 * \author Сергей Хватов <xbatob@grouptechno.ru>, (C) 2009
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gettext.h"

#ifndef LOCALEDIR
#define LOCALEDIR NULL
#endif

#ifndef PACKAGE_STRING
#define PACKAGE_STRING "template 0.0"
#define PACKAGE_BUGREPORT "Sergey Khvatov <xbatob@grouptechno.ru>"
#endif
#define MY_VERSION     PACKAGE_STRING \
    "\n$Id: $"

#include <locale.h>
#include <argp.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>

#include <libconfig.h++>

using namespace std;
using namespace libconfig;

const char *argp_program_version = MY_VERSION;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

const char *symbol = "config";
const char *coding = "utf-8";

void print_string (const char *str) {
    cout << "u'";
    for (; *str; ++str) {
        char c;
        switch (*str) {
        case '\n': c= 'n'; break;
        case '\t': c= 't'; break;
        case '\r': c= 'r'; break;
        case '\\': c= '\\'; break;
        case '\'': c= '\''; break;
        default: cout << *str; continue;
        }
        cout << '\\' << c;
    }
    cout << '\'';
}

void parse_setting (Setting const &set) {
    Setting::Type t = set.getType ();
    switch (t) {
    case Setting::TypeGroup:
        cout << '{' << endl;
        for (int i = 0; i < set.getLength(); ++i) {
            Setting const &s = set[i];
            print_string (s.getName());
            cout << ':';
            parse_setting (s);
            cout << ',' << endl;
        }
        cout << '}';
        break;
    case Setting::TypeArray:
    case Setting::TypeList:
        cout << '[' << endl;
        for (int i = 0; i < set.getLength(); ++i) {
            Setting const &s = set[i];
            parse_setting (s);
            cout << ',' << endl;
        }
        cout << ']';
        break;
    case Setting::TypeBoolean:
        cout << (bool) set ? "true" : "false";
        break;
    case Setting::TypeInt:
    case Setting::TypeInt64:
        cout << (int) set;
        break;
    case Setting::TypeString:
        print_string (set.c_str());
        break;
    case Setting::TypeFloat:
        cout << (double) set;
        break;
    }
}

void config2py (const char *filename) {
    Config cf;
    try {
        cf.readFile (filename);
    } catch (ConfigException const &e) {
        cerr << filename << ": load error: " << e.what() << endl;
        return;
    }

    if (coding && *coding)
        cout << "# -*- coding: " << coding <<" -*-" << endl;
    cout << symbol << '=';
    try {
        parse_setting (cf.getRoot());
    } catch (SettingException const &e) {
        cerr << e.what() << " on " <<  e.getPath() << endl;
    }
    cout << endl;
}

static error_t apf (int key, char *arg, struct argp_state *state) {
    switch (key) {
    case 's':
        symbol = arg;
        break;
    case 'c':
        coding = arg;
        break;
    case ARGP_KEY_ARG:
        config2py (arg);
        break;
    default:
        return  ARGP_ERR_UNKNOWN;
    }
    return 0;
}

char *argp_print_help (const char *fmt, ...) {
// fmt is always not literal so format __attribute__ is useless
    char *b;
    va_list al;
    va_start (al, fmt);
    vasprintf (&b, fmt, al);
    va_end (al);
    return b;
}

static char *aph (int key, const char *text, void *input) {
    switch (key) {
    case 's':
        return argp_print_help (text, symbol);
    case 'c':
        return argp_print_help (text, coding);
    }
    return (char *) text;
}

static const struct argp_option apo[] = {
    {
        "symbol-name", 's', "NAME", 0,
        N_ ("Symbol name (%s)")
    }, {
        "coding", 'c', "ENCODING", OPTION_ARG_OPTIONAL,
        N_ ("Set encoding comment (%s)"
        " Omit argument to disable the comment")
    }, {
        0
    }
};

static const struct argp ap = {
    apo, apf, N_ ("config file..."),
    N_ ("convert configuration from libconfig format"
    " to Python variable definition"),
    0, aph
};

int main (int argc, char *argv[]) {
#if         ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
#endif  //  ENABLE_NLS

    argp_parse (&ap, argc, argv, 0, 0, 0);
    return 0;
}
