namespace commands {
	void add(char* name, char* path);
	void clean();
	void current();
	void debug(char* key);
	void install(char** argv, int argc);
	void list();
	void move(char* name, char* path);
	void print(char* name);
	void reg(char* name);
	void rename(char* old_name, char* new_name);
	void remove(char* name, bool keep);
	void use(char* name);
}