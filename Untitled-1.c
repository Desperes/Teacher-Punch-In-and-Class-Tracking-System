prog_char dictEnglishSalami[] PROGMEM = "Salami";
prog_char dictEnglishProsecco[] PROGMEM = "Prosecco";
const char *string_table[] = {
   dictEnglishSalami,
   dictEnglishProsecco,
   ...
   };
Print(string_table[0][0])