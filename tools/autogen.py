import os, re, textwrap, argparse
from pathlib import Path


def text(s: str) -> str:
    return textwrap.dedent(s.strip('\n'))

def read_file(file_path: str) -> str:
    with open(file_path) as file:
        return file.read()

def write_file(file_path: str, text: str):
    with open(file_path, "w") as file:
        file.write(text)


class CTypedIdentifier():
    pattern = r"(?P<type>.+\W)(?P<name>\w+)"

    def __init__(self, type: str, name: str):
        self.type = type
        self.name = name

    def __str__(self):
        return self.type + " " + self.name

    def __repr__(self):
        return str(self)

    def parse(text: str) -> 'CTypedIdentifier':
        match = re.match(CTypedIdentifier.pattern, text.strip())
        return CTypedIdentifier(match.group('type').strip(), match.group('name').strip())


class CFunction():
    pattern = r"(?P<tag>[^\(\)\n]+)\((?P<args>[^\(\)]*)\);"

    def __init__(self, tag: CTypedIdentifier, args: list[CTypedIdentifier]):
        self.tag = tag
        self.args = args

    def __str__(self):
        return self.prototype()

    def __repr__(self):
        return str(self)

    def parse(text: str) -> 'CFunction':
        match = re.match(CFunction.pattern, text)

        name = CTypedIdentifier.parse(match.group('tag'))

        args = []
        if match.group('args') != 'void':
            for arg_text in match.group('args').split(','):
                args.append(CTypedIdentifier.parse(arg_text))

        return CFunction(name, args)

    def prototype(self) -> str:
        return f"{self.tag}({self.define_args()})"

    def function_type(self) -> str:
        return f"{self.tag.type}(*)({self.define_args()})"

    def has_void_args(self) -> bool:
        return len(self.args) == 0

    def define_args(self) -> str:
        if self.has_void_args():
            return "void"
        else:
            return ", ".join([str(arg) for arg in self.args])

    def pass_args(self) -> str:
        return ", ".join([arg.name for arg in self.args])

    def ignore_arguments(self) -> str:
        if len(self.args) == 0:
            return ""

        return ' '.join([f'(void) {arg.name};' for arg in self.args])

    def return_(self) -> str:
        if self.tag.type == "void":
            return ""
        else:
            return "return "

    def default_return_value(self) -> str:
        if self.tag.type == "void":
            return "/* do nothing by default */"
        elif self.tag.type == "fj_err_t":
            return "FJ_ERR_UNIMPLEMENTED"
        elif self.tag.type == "bool":
            return "false"
        elif self.tag.type.endswith('*'):
            return "NULL"
        else:
            return "0"

    def return_default(self):
        return f"{self.return_()}{self.default_return_value()}"

    def pointer_name(self) -> str:
        return '_' + self.tag.name + '_ptr'

    def callable_pointer(self) -> str:
        return f"({self.function_type()}){self.pointer_name()}"

    def call_pointer(self) -> str:
        return f"({self.callable_pointer()})({self.pass_args()})"

    def return_called_pointer(self) -> str:
        return f"{self.return_()}{self.call_pointer()}"

    def default_implementation(self) -> str:
        return text(f"""
            {self.prototype()}
            {{
                {self.ignore_arguments()}
                {self.return_default()};
            }}
        """)

    def loader_implementation(self) -> str:
        return text(f"""
            fj_loader_function_t {self.pointer_name()};
            {self.prototype()}
            {{
                if ({self.pointer_name()} != NULL) {{
                    {self.return_called_pointer()};
                }}
                {self.return_default()};
            }}
        """)


class CSource():
    def __init__(self, file_name: str, functions: list[CFunction]):
        self.functions = functions
        self.file_name = file_name

    def parse_file(file_path: str) -> 'CSource':
        functions = []
        for match in re.finditer(CFunction.pattern, read_file(file_path)):
            functions.append(CFunction.parse(match.group(0)))

        return CSource(Path(os.path.basename(file_path)).stem, functions)

    def default_implementation(self) -> str:
        return text("""
            {header}

            #include <fejix/interface/{file_name}.h>

            {default_implementations}
        """).format(
            header = Autogen.autogen_header,
            file_name = self.file_name,
            default_implementations = '\n'.join([
                function.default_implementation() for function in self.functions
            ])
        )

    def loader_implementation(self) -> str:
        return text("""
            {header}

            #include <src/loader/loader.h>

            #include <fejix/interface/{file_name}.h>

            {loader_implementations}
        """).format(
            header = Autogen.autogen_header,
            file_name = self.file_name,
            loader_implementations = '\n'.join([
                function.loader_implementation() for function in self.functions
            ])
        )


class Autogen():
    autogen_header = "/* Automatically generated by tools/autogen.py, do not edit! */"

    def __init__(self):
        script_dir = os.path.dirname(os.path.realpath(__file__))
        self.root_path = os.path.realpath(os.path.join(script_dir, ".."))

        self.input_path = os.path.join(self.root_path, "src", "headers", "fejix", "interface")
        self.default_output_path = os.path.join(self.root_path, "src", "default", "autogen")
        self.loader_output_path = os.path.join(self.root_path, "src", "loader", "autogen")

    def input_sources(self):
        for file_name in os.listdir(self.input_path):
            if os.path.isfile(os.path.join(self.input_path, file_name)):
                yield CSource.parse_file(os.path.join(self.input_path, file_name))


    def process_files(self):
        sources = list(self.input_sources())

        functions = []
        for source in sources:
            self.write_default_implementation(source)
            self.write_loader_implementation(source)
            functions += source.functions

        self.write_loader_function_list(functions)

    def write_default_implementation(self, source: CSource):
        file_path = os.path.join(self.default_output_path, source.file_name + '.c')
        text = source.default_implementation()
        write_file(file_path, text)

    def write_loader_implementation(self, source: CSource):
        file_path = os.path.join(self.loader_output_path, source.file_name + '.c')
        text = source.loader_implementation()
        write_file(file_path, text)

    def write_loader_function_list(self, functions: list[CFunction]):
        file_path = os.path.join(self.loader_output_path, '_loader_function_list.c')
        text = self.loader_function_list(functions)
        write_file(file_path, text)

    def loader_function_list(self, functions: list[CFunction]) -> str:
        return text("""
            {header}

            #include <src/loader.h>

            char const *fj_loader_function_names[] = {{
                {function_names},
            }};

            {functions};

            fj_loader_function_t *fj_loader_function_pointers[] = {{
                {function_pointers},
            }};

            uint32_t fj_loader_function_count = {function_count};
        """).format(
            header = Autogen.autogen_header,
            function_names = ',\n    '.join([
                '"' + function.tag.name + '"' for function in functions
            ]),
            functions = ';\n'.join([
                'extern fj_loader_function_t ' + function.pointer_name() for function in functions
            ]),
            function_pointers = ',\n    '.join([
                '(fj_loader_function_t*)&' + function.pointer_name() for function in functions
            ]),
            function_count = len(functions)
        )


if __name__ == "__main__":
    autogen = Autogen()
    autogen.process_files()

