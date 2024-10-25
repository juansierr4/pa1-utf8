I searched "overlong utf8 encoding problem" on safari and found this stackoverflow post: https://stackoverflow.com/questions/60838769/how-are-overlong-utf-8-encodings-generated

From there, I gathered info to better answer the last design question and learned about the security vulnerabilities and string comparison failures that come with mulitple encodings for the same character.