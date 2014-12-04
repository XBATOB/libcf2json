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

#ifndef PACKAGE
#define PACKAGE "libcf2json"
#endif

#ifndef PACKAGE_STRING
#define PACKAGE_STRING PACKAGE " 0.0"
#define PACKAGE_BUGREPORT "Sergey Khvatov <xbatob@grouptechno.ru>"
#endif
#define MY_VERSION     PACKAGE_STRING

#include <locale.h>
#include <argp.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>

#include <libconfig.h++>

#include <json/value.h>
#include <json/writer.h>

using namespace std;
using namespace libconfig;
using namespace Json;

const char *argp_program_version = MY_VERSION;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

Value topObject (objectValue);

void parse_setting (Setting const &set, Value &dest) {
    Setting::Type t = set.getType ();
    switch (t) {
    case Setting::TypeGroup:
        dest = Value (objectValue); // if group is empty
        for (int i = 0; i < set.getLength(); ++i) {
            Setting const &s = set[i];
            parse_setting (s, dest[s.getName()]);
        }
        break;
    case Setting::TypeArray:
    case Setting::TypeList:
        dest = Value (arrayValue); // if array is empty
        for (int i = 0; i < set.getLength(); ++i) {
            Setting const &s = set[i];
            parse_setting (s, dest[i]);
        }
        break;
    case Setting::TypeBoolean:
        dest = (bool) set;
        break;
    case Setting::TypeInt:
        dest = (int) set;
        break;
    case Setting::TypeInt64:
        dest = (long long int) set;
        break;
    case Setting::TypeString:
        dest = set.c_str();
        break;
    case Setting::TypeFloat:
        dest = (double) set;
        break;
    default:
        cerr << _ ("setting ignored: ") << set.getPath() << endl;
    }
}

const char *root;

void config2json (const char *filename) {
    Config cf;
    try {
        cf.readFile (filename);
    } catch (ParseException const &e) {
        cerr << filename << _ (": load error: ") << e.what() <<
             " at " << e.getFile() << " line " << e.getLine() << endl;
        return;
    } catch (ConfigException const &e) {
        cerr << filename << _ (": load error: ") << e.what() << endl;
        return;
    }

    try {
        Value root_val;
        parse_setting (root ? cf.lookup (root) : cf.getRoot(), root_val);
        for (Value::iterator p = root_val.begin();
                p != root_val.end();
                ++p) {
            topObject[p.memberName()] = *p;
        }
    } catch (SettingException const &e) {
        cerr << e.what() << " on setting " <<  e.getPath() << endl;
    }
}

static error_t apf (int key, char *arg, struct argp_state *state) {
    switch (key) {
    case 'o': {
        filebuf *fb = new filebuf;
        if (!fb->open (arg, ios_base::out | ios_base::trunc)) {
            argp_failure (state, 1, errno, _ ("Can not open %s"), arg);
        }
        cout.rdbuf (fb);
        break;
    }
    case 'r':
        root = arg;
        break;
    case ARGP_KEY_ARG:
        config2json (arg);
        break;
    case ARGP_KEY_END:
        cout << topObject;
    default:
        return  ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static const struct argp_option apo[] = {
    {
        "output", 'o', "FILE", 0,
        N_ ("Output to FILE")
    }, {
        "root", 'r', "SETTING", OPTION_ARG_OPTIONAL,
        N_ ("Convert only given subtree")
    }, {
        0
    }
};

static const struct argp ap = {
    apo, apf, N_ ("config file..."),
    N_ ("convert configuration from libconfig format"
    " to JSON one"),
    0, 0
};

int main (int argc, char *argv[]) {
#if         ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
#endif  //  ENABLE_NLS

    argp_parse (&ap, argc, argv, ARGP_IN_ORDER, 0, 0);
    return 0;
}
