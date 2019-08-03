/*
 * UNG's Not GNU
 *
 * Copyright (c) 2019, Jakob Kaivo <jkk@ung.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _POSIX_C_SOURCE 2
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

enum { SETMODE = 1 << 0, PARENTS = 1 << 1 };

static int translate_mode(const char *s)
{
	/* TODO: same as chmod */
	return 0;
}

static int mk_dir(char *path, int mode, int flags)
{
	/* TODO: parents */

	if (mkdir(path, mode) != 0) {
		fprintf(stderr, "mkdir: %s: %s\n", path, strerror(errno));
		return 1;
	}

	if ((flags & SETMODE) && (chmod(path, mode) != 0)) {
		fprintf(stderr, "mkdir: %s: %s\n", path, strerror(errno));
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	int mode = S_IRWXU | S_IRWXG | S_IRWXO;
	int flags = 0;

	int c;
	while ((c = getopt(argc, argv, "mp:")) != -1) {
		switch (c) {
		case 'm':
			mode = translate_mode(optarg);
			flags |= SETMODE;
			break;

		case 'p':
			flags |= PARENTS;
			break;

		default:
			return 1;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "mkdir: missing operand\n");
		return 1;
	}

	int ret = 0;
	do {
		ret |= mk_dir(argv[optind], mode, flags);
	} while (++optind < argc);

	return ret;
}
