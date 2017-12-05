#include<stdio.h>
#include<cstring>
#include<cstdlib>

char X[16];
char Y[16];
char vars[101][4];
int varCount = 0;

int main()
{
	//freopen("phalanx20.in", "r", stdin);
	//freopen("phalanx.out", "w", stdout);
	int t = 0;;
	scanf("%d", &t);
	bool ok = true;
	for (int i = 1; ok && i <= t; ++i)
	{
		int rows = 0;
		char O[16];
		scanf("%d%s", &rows, O);
		int o_value = 0;
		if (O[2] == 'n') {
			O[strlen(O) - 1] = 0;
			o_value = atoi(O + 4);
		}
		int level = 0;
		int ncounter = 0;
		int max_counter = 0;
		bool count_finished = false;
		for (int r = 1; ok && r <= rows; ++r) {
			char lineType[8];
			scanf("%s", lineType);
			if (strcmp(lineType,"F") == 0) {
				
				scanf("%s%s%s", vars[level], X, Y);
				for (int l = 0; l < level; ++l) {
					if (strcmp(vars[l], vars[level]) == 0) {
						ok = false;
						break;
					}
				}
				if (!count_finished) {
					if (strcmp(X, "n") == 0) {
						count_finished = true;
					}
					else {
						if (strcmp(Y, "n") == 0) {
							ncounter++;
							if (ncounter > max_counter) {
								max_counter = ncounter;
							}
						}
						else {
							int x = atoi(X);
							int y = atoi(Y);
							if (x > 0 && y > 0 && x >= y) {
								count_finished = true;
							}
						}
					}
				}
				level++;
			}
			else if (strcmp(lineType,"E") == 0) {
				level--;
				if (level == 0) {
					ncounter = 0;
				}
			}
			else {
				//ok = false;
				//break;
			}
		}
		if (!ok) {
			printf("ERR\n");
		}
		else if (level != 0) {
			printf("ERR\n");
		}
		else if(max_counter == o_value)
		{
			printf("Yes\n");
		}
		else {
			printf("No\n");
		}
	}
	//fclose(stdin);
	//fclose(stdout);

	return 0;
}