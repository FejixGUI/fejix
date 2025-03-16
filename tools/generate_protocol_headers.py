import argparse, os, pathlib, json, jinja2


PATH = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))

supported_formats = [
    "fejix_protocol-0.1"
]

builtin_types = {
    "int8": "int8_t",
    "uint8": "uint8_t",
    "int16": "int16_t",
    "uint16": "uint16_t",
    "int32": "int32_t",
    "uint32": "uint32_t",
    "int64": "int64_t",
    "uint64": "uint64_t",
    "bool": "bool",
    "string": "char const *",
    "time": "fj_time_t",
    "density": "fj_density_t",
    "color_format": "fj_color_format_t",
    "io_element": "struct fj_io_element *",
    # Structs like 'size' or 'rect' work just normally when using 'struct:' notation
}


def main():
    argparser = argparse.ArgumentParser(
        prog = "generate_protocol_headers.py",
        usage = "python3 generate_protocol_headers.py -d DESTDIR PROTOCOL1.json PROTOCOL2.json...",
        description = "Generate C header files from Fejix protocol specifications"
    )

    argparser.add_argument('-d', '--destdir', required = True)
    argparser.add_argument('protocol_files', nargs = "+")

    args = argparser.parse_args()

    env = jinja2.Environment(
        loader = jinja2.FileSystemLoader(PATH),
        trim_blocks = False,
        lstrip_blocks = True
    )

    template = env.get_template("protocol_header.h.jinja")

    for protocol_file in args.protocol_files:
        print("Processing " + protocol_file + "...")

        try:
            file_stem = pathlib.Path(protocol_file).stem
            header_file = os.path.join(args.destdir, file_stem + ".h")

            protocol = json.load(open(protocol_file))

            if "format" not in protocol or "name" not in protocol or "id" not in protocol:
                raise Exception("File missing required attributes: 'format', 'name', 'id'")

            if protocol["format"] not in supported_formats:
                raise Exception("Unsupported format '" + protocol["format"] + "'")

            output = template.render(
                protocol = protocol,
                enum_without_suffix = enum_without_suffix,
                annotate_with_type = annotate_with_type
            )

            open(header_file, "w").write(output)

        except Exception as e:
            print('[Error] ' + e.args[0])
            print('[Warning] File skipped')


def enum_without_suffix(enum: dict) -> str:
    if "suffix" not in enum:
        return enum["name"]

    return enum["name"][ : -len(enum["suffix"])]


def alphabetic_separator(type_string: str) -> str:
    if type_string[-1].isalpha():
        return " "

    return ""

def star_separator(type_string: str) -> str:
    if type_string[-1] == "*":
        return ""

    return " "

# arg may be a struct field, event arg or method arg of form:
#   [module::[typeclass:]]type
# Examples:
#   bool
#   uint32
#   struct:my_struct
#   some_protocol::enum:my_enum
def annotate_with_type(arg: dict) -> str:
    out = ""

    type = arg["type"]
    typeclass = ""
    _module = ""

    if "::" in type:
        (_module, type) = type.split("::")

    if ":" in type:
        (typeclass, type) = type.split(":")

    if typeclass == "struct":
        out += "struct fj_" + type
    elif typeclass == "returned_object":
        out += "struct fj_" + type + " *"
    elif typeclass == "enum":
        out += "fj_" + type + "_t"
    elif typeclass == "":
        if type in builtin_types:
            out += builtin_types[type]
        else:
            raise Exception("Unknown type '" + type + "' in '" + arg["type"] + "'")
    else:
        raise Exception("Unknown typeclass '" + typeclass + "' in '" + arg["type"] + "'")

    if typeclass == "struct":
        if not "out" in arg:
            out += alphabetic_separator(out)
            out += "const"

    if "out" in arg:
        out += star_separator(out)
        out += "*"

    out += alphabetic_separator(out)

    if "optional" in arg:
        out += "opt_"

    if "out" in arg:
        out += "out_"

    out += arg["name"]

    return out


if __name__ == "__main__":
    main()