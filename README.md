# el - an ansi c l-system library

<p align="center">
  <img alt="Kock curve example" width=400 src="https://github.com/hatcatpat/el/assets/39860407/48f01ff9-55a4-48be-ab38-b15eadd99e75"/>
</p>

## usage
```c
el_system_t* L = el_create(); /* allocates symbol/rule dictionaries */
```

```c
void callback(char *output, int position);
el_add_symbol(L, "F", callback); /* creates a symbol, and asigns it a callback function */
el_add_symbol(L, "+", NULL); /* creates a symbol, with no callback function */
```

```c
el_add_rule(L, "F+", "+F"); /* adds the rule F+ -> +F */
el_add_rule(L, "F", "F+F"); /* adds the rule F -> F+F */
/* note, earlier rules take precedence */
/* so, if we switched the order of these rules, only F -> F+F would occur */
```

```c
char *output = el_perform(L, "F", 4); /* performs the L-system, with 4 iterations */
/* note, el is responsible for allocating/freeing the output string */
```

```c
el_enact(L, output); /* iterates over the string, calling the functions asigned to each symbol */
```

```c
el_free(L); /* frees the symbol/rule dictionaries, and also frees the output */
```  

