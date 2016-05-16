/*
 * Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>
#include <set>

/*
 Configuration file format

 # comment...

 D source_directory dest_directory  namespace:identifier
 T textual-source
 B binary-source
 
 */

using namespace std;

// ----------------------------------------------------------------------------
// MARK: Globals -
// ----------------------------------------------------------------------------

static string prog_name;



// ----------------------------------------------------------------------------
// MARK: Helper functions
// ----------------------------------------------------------------------------

static void SplitString(const string & str, char delim, vector<string> & elems)
{
	elems.clear();
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) {
			elems.push_back(item);
		}
	}
}

static void AppendPath(string & path, const string & new_segment)
{
	bool sep_new = !new_segment.empty() && new_segment.front() == '/';
	bool sep_cur = !path.empty() && path.back() == '/';
	if (sep_new == sep_cur) {
		if (sep_cur) {
			// both have separator, remove from current
			path.pop_back();
		} else {
			// none of them have separator
			path.append("/");
		}
	} else {
		// new or current has separator, don't care, just append
	}
	path.append(new_segment);
}

static string MakeVariable(const string & identifier)
{
	string res;
	res.reserve(identifier.length());
	for (char c : identifier) {
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z') || (c == '_')) {
			res.push_back(c);
		} else {
			res.push_back('_');
		}
	}
	return res;
}

static string EscapeCString(const string & str)
{
	string res;
	res.reserve(str.length());
	for (char c : str) {
		if (c == '"' || c == '\\') {
			res.push_back('\\');
		}
		res.push_back(c);
	}
	return res;
}

static bool ValidateVariable(const string & identifier)
{
	for (char c : identifier) {
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z') ||(c == '_')) {
			continue;
		} else {
			return false;
		}
	}
	return true;
}



// ----------------------------------------------------------------------------
// MARK: Model -
// ----------------------------------------------------------------------------

struct FileEntry
{
	string	source;
	string	identifier;
	bool	textual;
	
	string	gen_var_name;
	string	gen_source_full_path;
	
	FileEntry() {
		textual = true;
	}
	
	bool is_valid() const 
	{
		return !source.empty() && !identifier.empty();
	}
	
	bool validate_identifier(const string & dir_path)
	{
		gen_var_name = MakeVariable(identifier);
		gen_source_full_path = dir_path;
		AppendPath(gen_source_full_path, source);
		return true;
	}
};

struct Directory
{
	string		source;
	string		destination;
	string		identifier;
	
	vector<FileEntry> files;
	set<string> file_identifiers;
	
	vector<string> gen_namespaces;
	string         gen_identifier;
	
	void reset() 
	{
		source.clear();
		destination.clear();
		identifier.clear();
		files.clear();
		gen_namespaces.clear();
		gen_identifier.clear();
	}
	
	bool is_valid() const 
	{
		return !source.empty() && !destination.empty() && !identifier.empty();
	}
	
	bool validate_namespace() 
	{
		SplitString(identifier, ':', gen_namespaces);
		if (gen_namespaces.empty()) {
			fprintf(stderr, "%s: Directory '%s' has empty or invalid namespace '%s'.\n", prog_name.c_str(), source.c_str(), identifier.c_str());
			return false;
		}
		for (string & ns : gen_namespaces) {
			if (!ValidateVariable(ns)) {
				fprintf(stderr, "%s: Directory '%s' has invalid namespace '%s'.\n", prog_name.c_str(), source.c_str(), ns.c_str());
				return false;
			}
		}
		gen_identifier = gen_namespaces.back();
		gen_namespaces.pop_back();
		return true;
	}
};

struct Configuration
{
	int			current_line;
	string		config_file_name;
	
	// Current dir
	Directory			current_dir;
	vector<Directory>	directories;
	set<string>			directory_identifiers;
	
	Configuration() {
		current_line = 1;
	}
};



// ----------------------------------------------------------------------------
// MARK: main() -
// ----------------------------------------------------------------------------

static int load_config_file(FILE * cf, Configuration & conf);
static int process_configuration(Configuration & conf);
static void print_help();

int main(int argc, char* argv[])
{
	int result = 0;
	
	Configuration config;
	
	for (int i = 0; i < argc; i++) {
		string param(argv[i]);
		if (i == 0) {
			prog_name = param;
			continue;
		}
		if (param == "--test") {
			// self test
			return 42;
			//
		} else if (param == "-h" || param == "--help") {
			print_help();
			return 1;
			//
		} else if (param == "-pn" || param == "--prog-name") {
			//
			if (i + 1 < argc) {
				prog_name = argv[++i];
				continue;
			} else {
				fprintf(stderr, "%s: Missing program name.\n", prog_name.c_str());
				return 1;
			}
			//		
		} else if (i + 1 == argc) {
			// last parameter, keep config file name
			config.config_file_name = param;
			break;
			//
		} else {
			fprintf(stderr, "%s: Unknown parameter '%s'.\n", prog_name.c_str(), param.c_str());
			print_help();
			return 1;
		}
	}
		
	if (config.config_file_name.empty()) {
		fprintf(stderr, "%s: The config file was not specified.\n", prog_name.c_str());
		return 1;
	}
	
	FILE * conf_file = fopen(config.config_file_name.c_str(), "r");
	if (!conf_file) {
		fprintf(stderr, "%s: Unable to open config file: %s\n", prog_name.c_str(), config.config_file_name.c_str());
		return 1;
	}
	
	
	result = load_config_file(conf_file, config);
	fclose(conf_file);
	
	if (result == 0) {
		result = process_configuration(config);
	}
	
	return result;
}


// Help printing

static void print_help()
{
	fprintf(stdout, "Usage:  %s  --help | -h | config_file\n", prog_name.c_str());
}



// ----------------------------------------------------------------------------
// MARK: Config file parser -
// ----------------------------------------------------------------------------

static int process_line(const string & line, Configuration & conf);

static int load_config_file(FILE * cf, Configuration & conf)
{
	char line_buffer[1024];
	string line;
	line.reserve(1024);
	int result = 0;
	while (NULL != fgets(line_buffer, sizeof(line_buffer), cf)) {
		line.assign(line_buffer);
		result = process_line(line, conf);
		if (result != 0) {
			break;
		}
		conf.current_line++;
	}
	if (conf.current_dir.is_valid()) {
		conf.directories.push_back(conf.current_dir);
	}
	return result;
}


static int process_line(const string & line, Configuration & conf)
{
	char command[16];
	char source[256];
	char destination[256];
	char identifier[256];
	
	if (line.size() > 0) {
		// Very naive line parser...
		int count = sscanf(line.c_str(), "%15s %255s %255s %255s", command, source, destination, identifier);
		if (count == EOF) {
			// probably empty line
			return 0;
		}
		if (count > 0) {
			string cmd(command);
			if (cmd == "#" || cmd == "//") {
				// newline
				return 0;
			}
			if (cmd == "D" || cmd == "Directory") {
				
				if (count >= 4) {
					if (conf.current_dir.is_valid()) {
						conf.directories.push_back(conf.current_dir);
						conf.current_dir.reset();
					}
					conf.current_dir.source.assign(source);
					conf.current_dir.destination.assign(destination);
					conf.current_dir.identifier.assign(identifier);
					return 0;
					
				} else {
					fprintf(stderr, "%s: %s: Insufficient number of parameters for directory at line %d.\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line);
					return 1;
				}
				
			} else if (cmd == "T" || cmd == "Text" || cmd == "B" || cmd == "Binary") {

				FileEntry fe;
				fe.textual = command[0] == 'T';
				
				if (conf.current_dir.is_valid()) {
					if (count >= 2) {
						fe.source.assign(source);
						if (count >= 3) {
							fe.identifier.assign(destination);
						} else {
							fe.identifier = fe.source;
						}
						conf.current_dir.files.push_back(fe);
						return 0;
						
					} else {
						fprintf(stderr, "%s: %s: Insufficient number of parameters for file at line %d.\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line);
						return 1;
					}
					
				} else {
					fprintf(stderr, "%s: %s: File without directory at line %d.\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line);
					return 1;
				}
				
			} else {
				if (command[0] == '#' || (command[0] == '/' && command[1] == '/')) {
					// comment without space after hash or double slash
					return 0;
				}
			}
			
			fprintf(stderr, "%s: %s: Unknown token at line %d\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line);
			return 1;
			
		} else {
			fprintf(stderr, "%s: %s: An unknown error occured during the config file processing. Line %d. %d\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line, count);
			return 1;
		}
	}
	return -1;
}



// ----------------------------------------------------------------------------
// MARK: Directory generator -
// ----------------------------------------------------------------------------

static int write_file(FILE * FC, FileEntry & file);

#define write_line(f) fprintf(f, "// -----------------------------------------------------------------------------\n");

static int process_configuration(Configuration & conf)
{
	// Validate duplicities & other checks
	for (Directory & dir : conf.directories) {
		if (!dir.is_valid()) {
			fprintf(stderr, "%s: %s: Directory is not valid (internal error)\n", prog_name.c_str(), conf.config_file_name.c_str());
			return -1;
		}
		if (conf.directory_identifiers.find(dir.identifier) != conf.directory_identifiers.end()) {
			fprintf(stderr, "%s: %s: Duplicit directory with identifier '%s'\n", prog_name.c_str(), conf.config_file_name.c_str(), dir.identifier.c_str());
			return -1;
		}
		conf.directory_identifiers.insert(dir.identifier);
		if (!dir.validate_namespace()) {
			return -1;
		}
		for (FileEntry & file : dir.files) {
			if (!file.is_valid()) {
				fprintf(stderr, "%s: %s: File is not valid (internal error)\n", prog_name.c_str(), conf.config_file_name.c_str());
				return -1;
			}
			if (!file.validate_identifier(dir.source)) {
				return -1;
			}
			if (dir.file_identifiers.find(file.identifier) != dir.file_identifiers.end()) {
				fprintf(stderr, "%s: %s: Directory '%s' contains duplicit file '%s'\n", prog_name.c_str(), conf.config_file_name.c_str(), dir.identifier.c_str(), file.identifier.c_str());
				return -1;
			}
			dir.file_identifiers.insert(dir.identifier);
		}
	}
	// Now generate files
	for (Directory & dir : conf.directories) {
		// Prepare file name for target cpp file
		string dir_content_file_name = dir.destination;
		AppendPath(dir_content_file_name, dir.gen_identifier);
		dir_content_file_name.append(".cpp");
		
		// Open text file for writing
		printf("## DIR >> %s\n", dir_content_file_name.c_str());
		FILE * DC = fopen(dir_content_file_name.c_str(), "w");
		if (!DC) {
			fprintf(stderr, "%s: Unable to open file for writing: %s\n", prog_name.c_str(), dir_content_file_name.c_str());
			return -1;
		}
		// Write preamble
		fprintf(DC, "// Generated code - DO NOT EDIT !!\n");
		fprintf(DC, "// Generated by '%s'\n\n", prog_name.c_str());
		
		fprintf(DC, "#include <cc7tests/TestDirectory.h>\n");
		fprintf(DC, "#include <cc7tests/TestResource.h>\n\n");
		
		// Open namespaces
		string close_namespaces;
		string full_namespace;
		for (string & ns : dir.gen_namespaces) {
			fprintf(DC, "namespace %s\n{\n", ns.c_str());
			close_namespaces.insert(0, "} // " + ns + "\n");
			if (!full_namespace.empty()) {
				full_namespace.append("::");
			}
			full_namespace.append(ns);
		}
		
		std::string help_first_file;
		if (!dir.files.empty()) {
			help_first_file = dir.files.front().gen_source_full_path;
		}
		fprintf(DC, "/*\n");
		fprintf(DC, "  Available files:\n\n");
		for (FileEntry & file : dir.files) {
			fprintf(DC, "     * %s\n", file.gen_source_full_path.c_str());
		}	
		fprintf(DC, "\n");
		fprintf(DC, "  Usage:\n");
		fprintf(DC, "\n");
		fprintf(DC, "    #include <cc7tests/TestDirectory.h>\n");
		fprintf(DC, "\n");
		fprintf(DC, "    namespace %s {\n", full_namespace.c_str());
		fprintf(DC, "       extern cc7::tests::TestDirectory %s;\n", dir.gen_identifier.c_str());
		fprintf(DC, "    }\n");
		fprintf(DC, "\n");
		fprintf(DC, "    void someTestFunction() {\n");
		fprintf(DC, "       cc7::tests::TestFile f = %s.findFile(\"%s\");\n", dir.gen_identifier.c_str(), help_first_file.c_str());
		fprintf(DC, "       // Do whatever you want with TestFile object\n");
		fprintf(DC, "       // You don't need to close file or directory\n");
		fprintf(DC, "    }\n");
		fprintf(DC, " */\n\n");		
				
		// Generate content of files
		for (FileEntry & file : dir.files) {
			if (0 != write_file(DC, file)) {
				fclose(DC);
				return -1;
			}
		}
		
		// Generate forward declarations for files
		write_line(DC);
		fprintf(DC, "// Directory: %s\n", dir.source.c_str());
		write_line(DC);
		// Directory instantiation
		fprintf(DC, "//\ncc7::tests::TestDirectory %s({\n", dir.gen_identifier.c_str());
		for (FileEntry & file : dir.files) {
			fprintf(DC, "\t&%s,\n", file.gen_var_name.c_str());
		}		
		fprintf(DC, "});\n\n");
		
		
		// Close namespaces
		fprintf(DC, "%s", close_namespaces.c_str());
		// And finally, close file
		fclose(DC);
	}
	return 0;
}



// ----------------------------------------------------------------------------
// MARK: Data files generator -
// ----------------------------------------------------------------------------

int write_textual(FILE * OUT, FILE * SRC, FileEntry & file, const string & var_bytes, const string & var_size);
int write_binary(FILE * OUT, FILE * SRC, FileEntry & file, const string & var_bytes, const string & var_size);

int write_file(FILE * OUT, FileEntry & file)
{
	const char * mode = file.textual ? "r" : "rb";
	
	printf("   + FILE: %s\n", file.gen_source_full_path.c_str());
	FILE * SRC = fopen(file.gen_source_full_path.c_str(), mode);
	if (!SRC) {
		fprintf(stderr, "%s: Unable to open file for writing: %s\n", prog_name.c_str(), file.gen_source_full_path.c_str());
		return -1;
	}
	// File preamble
	write_line(OUT);
	fprintf(OUT, "// File: %s\n", file.gen_source_full_path.c_str());
	
	string var_bytes = file.gen_var_name + "_bytes";
	string var_size = file.gen_var_name + "_size";
	int result;
	if (file.textual) {
		result = write_textual(OUT, SRC, file, var_bytes, var_size);
	} else {
		result = write_binary(OUT, SRC, file, var_bytes, var_size);
	}
	// File instantiation
	fprintf(OUT, "//\n");
	fprintf(OUT, "static cc7::tests::TestResource %s\n", file.gen_var_name.c_str());
	fprintf(OUT, "\t(\n");
	fprintf(OUT, "\t\treinterpret_cast<const cc7::byte*>(%s),\n", var_bytes.c_str());
	fprintf(OUT, "\t\t%s,\n", var_size.c_str());
	fprintf(OUT, "\t\t\"%s\"\n", EscapeCString(file.gen_source_full_path).c_str());
	fprintf(OUT, "\t);\n");
	fprintf(OUT, "//\n");
	fclose(SRC);
	return result;
}

int write_textual(FILE * OUT, FILE * SRC, FileEntry & file, const string & var_bytes, const string & var_size)
{
	string begin_token = "R\"cc7resourcxx(";
	string end_token   = ")cc7resourcxx\"";
	
	int result = 0;
	write_line(OUT);
	fprintf(OUT, "static const char %s[] = %s", var_bytes.c_str(), begin_token.c_str());
	int number_of_bytes = 0;
	while (1) {
		int c = fgetc(SRC);
		if (c != EOF) {
			fputc(c, OUT);
			number_of_bytes++;
		} else {
			if (ferror(SRC)) {
				fprintf(stderr, "%s: Unable to read content of file: %s\n", prog_name.c_str(), file.gen_source_full_path.c_str());
				result = -1;
			}
			break;
		}
	}
	fprintf(OUT, "%s;\n", end_token.c_str());
	fprintf(OUT, "static const size_t %s = %d;\n", var_size.c_str(), number_of_bytes);
	write_line(OUT);
	return result;
}

int write_binary(FILE * OUT, FILE * SRC, FileEntry & file, const string & var_bytes, const string & var_size)
{
	int result = 0;
	write_line(OUT);
	fprintf(OUT, "static const cc7::byte %s[] = {", var_bytes.c_str());
	int number_of_bytes = 0;
	int column = 0;
	while (1) {
		uint8_t byte;
		if (1 == fread(&byte, sizeof(byte), 1, SRC)) {
			number_of_bytes++;
			if (column == 0) {
				fprintf(OUT, "\n0x%02x,", byte);
			} else {
				fprintf(OUT, "0x%02x,", byte);
			}
			if (++column == 16) {
				column = 0;
			}
		} else {
			if (ferror(SRC)) {
				fprintf(stderr, "%s: Unable to read content of file: %s\n", prog_name.c_str(), file.gen_source_full_path.c_str());
				result = -1;
			}
			break;
		}
	}
	fprintf(OUT, "};\n");
	fprintf(OUT, "static const size_t %s = %d;\n", var_size.c_str(), number_of_bytes);
	write_line(OUT);
	return result;
}
