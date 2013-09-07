/*
 * Copyright (C) 2013 Esteban Ruiz Moreno <exio4.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


//  uber-wip code, it may (or not) work
//  based on:
//      http://awk.info/?doc/bfc.html

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STACK_SIZE 30000

int run(char *string, int len, int stack[], int c, int loop) {
    int i,tmp;
    int il = -1;


    for (i=0; i<len; i++) {
        switch (string[i]) {
            case '[':
                il = i+1;
                break;
            case ']':
                if (il<0) return -1;

                tmp = i - il;
                tmp = run(&string[il], tmp, stack, c, 1);

                if (tmp!=0) return -1;

                c = tmp;
                il = -1;
                break;
            default:
                {
                    // inside a 'loop'? skip this, it'll run as soon as this gets the ]
                    if (il>0) continue;
                    // interpreter shit here
                    switch(string[i]) {
                        case '+':
                            ++stack[c];
                            break;
                        case '-':
                            --stack[c];
                            break;
                        case '>':
                            ++c;
                            break;
                        case '<':
                            --c;
                            break;
                        case '.':
                            putchar(stack[c]);
                            break;
                        case ',':
                            stack[c] = getchar();
                            break;
                        default:
                            // should make a proper "error show" here
                            return -1;
                    }
                }
        }
    }

    if (loop) {
        if (stack[c] != 0) {
            c = run(string, len, stack, c, 1);
        }
    }
    return c;
}



int main(int argc, char** argv) {

    int stack[STACK_SIZE];

    if (argc < 2) {
        //usage(argv[1]);
        return -1;
    }

    run(argv[1], strlen(argv[1]), stack, 0, 0);

    return 0;
}

