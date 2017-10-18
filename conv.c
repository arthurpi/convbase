#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIN		"01"
#define DEC		"0123456789"
#define OCT		"01234567"
#define HEX		"0123456789ABCDEF"

/**
 * retrieve the index of a digit relative to its base
 */
static size_t get_digit_index(char c, char const *base)
{
	for (size_t i = 0; base[i]; i++) {
		if (base[i] == c) {
			return i;
		}
	}

	/* unknown character, the result will be wrong, so we exit now */
	fprintf(stderr, "character %c not found in base \"%s\"\nexiting...\n",
			c, base);

	exit(EXIT_FAILURE);
}

/**
 * convert a string to its integer equivalent, using the given input-base
 */
static uint64_t get_nbr_base(char *s, char const *base)
{
	uint64_t nb = 0;
	size_t base_len = strlen(base);

	for (size_t i = 0; s[i]; i++) {
		nb = (nb * base_len) + get_digit_index(s[i], base);
	}

	return nb;
}

/**
 * convert an integer to a string equivalent, using the given output-base
 *
 * note: it is the caller's responsability to free the value returned by this
 *       function
 */
static char *conv_to_base(uint64_t nb, char const *base)
{
	char *res;
	uint64_t nb_cpy = nb;
	size_t len_res = 0, len_base = strlen(base);

	for (; nb_cpy; nb_cpy /= len_base, len_res++);

	res = (char *) malloc(sizeof(char) * (len_res + 1));
	if (!res) {
		fprintf(stderr, "memory exhausted\nexiting...\n");
		exit(EXIT_FAILURE);
	}

	memset(res, 0, len_res + 1);

	for (int i = len_res - 1; i >= 0; i--) {
		res[i] = base[nb % len_base];
		nb /= len_base;
	}

	return res;
}

/**
 * transform each tiny char to capital char
 */
static void uniformize_hex(char *s)
{
	while (*s) {
		if (*s >= 'a' && *s <= 'z')
			*s -= 32;
		s++;
	}
}

static char const *guess_base(char **s)
{
	size_t len = strlen(*s);

	if (len < 1)
		return NULL;

	if (**s == '0' && len > 1) {
		if (*(*s + 1) == 'x') {
			*s += 2;
			uniformize_hex(*s);
			return HEX;
		} else if (*(*s + 1) == 'b') {
			*s += 2;
			return BIN;
		} else {
			*s += 1;
			return OCT;
		}
	}

	/* the default base */
	return DEC;
}

/**
 * helper function that prints a binary number and adds separation every 4 bits
 */
void print_bin(char const *s)
{
	size_t len = strlen(s);
	size_t init_shift = len % 4;

	printf("BIN: ");

	for (size_t i = 0; i < len; i++) {
		if (i && !((i - init_shift) % 4)) {
			putchar(' ');
		}
		putchar(s[i]);
	}

	putchar('\n');
}

static void print_usage_error(char const *s)
{
	fprintf(stderr, "usage: %s VALUE (must be >= 0)\n", s);
	fprintf(stderr, "for base recognition, prefix the VALUE with one of "\
			"the following\n"\
			"\tbinary     : 0b<%s>\n"\
			"\toctal      : 0<%s>\n"\
			"\tdecimal    : <%s>\n"\
			"\thexadecimal: 0x<%s>\n", BIN, OCT, DEC, HEX);
}

int main(int ac, char **av)
{
	uint64_t res;
	char *to_base;
	char const *from_base;

	if (ac < 2 || strlen(av[1]) == 0) {
		print_usage_error(*av);
		return EXIT_FAILURE;
	}

	from_base = guess_base(av + 1);
	res = get_nbr_base(av[1], from_base);
	printf("converting value >> %s << (input base: %s)\n", av[1], from_base);

	to_base = conv_to_base(res, BIN);
	print_bin(to_base);
	free(to_base);

	to_base = conv_to_base(res, OCT);
	printf("OCT: 0%s\n", to_base);
	free(to_base);

	to_base = conv_to_base(res, DEC);
	printf("DEC: %s\n", to_base);
	free(to_base);

	to_base = conv_to_base(res, HEX);
	printf("HEX: 0x%s\n", to_base);
	free(to_base);

	return EXIT_SUCCESS;
}
