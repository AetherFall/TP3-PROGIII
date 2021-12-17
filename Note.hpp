#pragma once

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Note.hpp
    @note object note
 */

using namespace std;

class Note{
private:
    string name;
    string content;

public:
	Note(string name, string content = ""){
        this->name = move(name);
        this->content = move(content);
    }

    string getName(){ return this->name; }
    string getContent() { return this->content; }

    void setName(string name) { this->name = move(name); }
    void setContent(string content) { this->content = move(content); }

};
