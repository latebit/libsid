#ifndef TEST_H
#define TEST_H

void expectEqlInt(int a, int b, char *msg);
void expectEqlString(char *a, char *b, char *msg);
void expect(bool condition, const char *name, const char *message);

int report();
#endif