import argparse, os, pathlib, json, jinja2
import traceback

PATH = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))

supported_formats = [
    "fejix_protocol-0.1"
]

builtin_types = {
    "any": "void const *",
    "anymut": "void *",
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
    "stringmut": "char *",
}

type_containers = {
    "ref": "const *",
    "refmut": "*",
    "array": "const *",
    "arraymut": "*",
    "opt_ref": "const *",
    "opt_refmut": "*",
    "opt_array": "const *",
    "opt_arraymut": "*",
}


def main():
    argparser = argparse.ArgumentParser(
        prog = "generate_protocol_headers.py",
        usage = "python3 generate_protocol_headers.py -d DESTDIR PROTOCOL1.json PROTOCOL2.json...",
        description = "Generate C header files from Fejix protocol specifications"
    )

    argparser.add_argument('-d', '--destdir', required = True)
    argparser.add_argument('-v', '--verbose', action = "store_true")
    argparser.add_argument('protocol_files', nargs = "+")

    args = argparser.parse_args()

    env = jinja2.Environment(
        loader = jinja2.FileSystemLoader(PATH),
        trim_blocks = True,
        lstrip_blocks = True
    )

    template = env.get_template("protocol_header.h.jinja")

    for protocol_file in args.protocol_files:
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
                make_type = make_type
            )

            open(header_file, "w").write(output)

        except Exception as e:
            print("Error in '" + str(protocol_file) + "': " + ", ".join(e.args))
            print("File '" + str(protocol_file) + "' skipped due to errors")

            if args.verbose:
                traceback.print_exception(e)
            else:
                print("Use --verbose to print the full traceback")



def enum_without_suffix(enum: dict) -> str:
    if "suffix" not in enum:
        return enum["name"]

    return enum["name"][ : -len(enum["suffix"])]


# Accepts a list of type components (e.g. ["opt_array", "refmut", "mytype"])
# Returns a corresponding C type string (e.g. fj_mytype_t * const*)
def make_type(type_spec: list, output_argument: bool = False) -> str:
    out = ""

    for index, component in enumerate(reversed(type_spec)):
        if index == 0:
            if component in builtin_types:
                out += builtin_types[component]
                continue

            out += "fj_" + component + "_t"

            continue

        if component in type_containers:
            out += " " + type_containers[component]
            continue

        raise Exception("Unknown type '" + component + "' in '" + ",".join(type_spec) + "'")

    if output_argument:
        out += "*"

    return out


if __name__ == "__main__":
    main()