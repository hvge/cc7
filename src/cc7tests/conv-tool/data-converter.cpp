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
#include <string>
#include <vector>
#include <set>

/*
 Configuration file format

 # comment...

 D source_directory dest_directory [ dir_name ]
 T textual-source dest.cpp [ identifier ]
 B binary-source  dest.cpp [ identifier ]
 
 */

using namespace std;

static void print_help();
static string prog_name;

struct FileEntry
{
	string	source;
	string	destination;
	string	identifier;
	
	bool	textual;
	
	FileEntry() {
		textual = true;
	}
};

struct Directory
{
	string		source;
	string		destination;
	string		identifier;
	
	vector<FileEntry> files;
	set<string> file_identifiers;
	
	void reset() {
		source.clear();
		destination.clear();
		identifier.clear();
		files.clear();
	}
	
	bool is_valid() const {
		return !source.empty() && !destination.empty();
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

static int load_config_file(FILE * cf, Configuration & conf);
static int process_configuration(Configuration & conf);

//
// main()
//
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

//
// Help printing
//
static void print_help()
{
	fprintf(stdout, "Usage:  %s  --help | -h | config_file\n", prog_name.c_str());
}

//
// Config file parser
//

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
				
				if (count >= 3) {
					if (conf.current_dir.is_valid()) {
						conf.directories.push_back(conf.current_dir);
						conf.current_dir.reset();
					}
					conf.current_dir.source.assign(source);
					conf.current_dir.destination.assign(destination);
					if (count >= 4) {
						conf.current_dir.identifier.assign(identifier);
					}
					return 0;
					
				} else {
					fprintf(stderr, "%s: %s: Insufficient number of parameters at line %d.\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line);
					return 1;
				}
				
			} else if (cmd == "T" || cmd == "Text" || cmd == "B" || cmd == "Binary") {

				FileEntry fe;
				fe.textual = command[0] == 'T';
				
				if (conf.current_dir.is_valid()) {
					if (count >= 3) {
						fe.source.assign(source);
						fe.destination.assign(destination);
						if (count >= 4) {
							fe.identifier.assign(identifier);
						}
						conf.current_dir.files.push_back(fe);
						return 0;
						
					} else {
						fprintf(stderr, "%s: %s: Insufficient number of parameters at line %d.\n", prog_name.c_str(), conf.config_file_name.c_str(), conf.current_line);
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

//
// Config file processor
//

static int process_configuration(Configuration & conf)
{
	printf("Dumping content...\n");
	return 0;
}