int find_redirect( char * arg ) {
  if ( strchr(arg, '>') || strchr(arg, '<') || strstr(arg, "2>") || strchr(arg, '|') ) {
    return 1;
  }
  return 0;
}

void parse_redirect(char * command) {
  int i = 0;
  while (command) {

  }
}
