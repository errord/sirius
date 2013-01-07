#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libsirius_suffixtree.h>

static void out_fcs_str(sarray_s *array)
{
	string16 *s16;
	char *buf;
	printf("Fcs String:\n");
	sarray_each_type_m(s16, string16*, array) {
		buf = s16_out_to_buff(s16);
		if (buf == NULL) {
			printf("error string!!!\n");
			continue;
		}
		printf("string: %s\n", buf);
		free(buf);
	}
	sarray_each_type_end_m;
}

static void out_fcs_str_to_file(FILE *fd, sarray_s *array)
{
	string16 *s16;
	char *buf;
	sarray_each_type_m(s16, string16*, array) {
		buf = s16_out_to_buff(s16);
		if (buf == NULL) {
			continue;
		}
		fprintf(fd, "%s\n", buf);
		free(buf);
	}
	sarray_each_type_end_m;
}

static void out_fcs_array(sarray_s *array)
{
	fcs_item item;
	char *buf;
	printf("Fcs Array:\n");
	sarray_each_type_m(item, fcs_item, array) {
		buf = s16_out_to_buff(suffixtree_fcs_item_string(item));
		if (buf == NULL) {
			printf("error string!!!\n");
			continue;
		}
		printf("fcs-> [state:0x%x][string:%s] [sidx:%d] [eidx:%d] [leafTotal:%d] [stringTotal:%d] [score:%f]\n",
			suffixtree_fcs_item_state(item),
			buf,
			suffixtree_fcs_item_startidx(item),
			suffixtree_fcs_item_endidx(item),
			suffixtree_fcs_item_leaf_total(item),
			suffixtree_fcs_item_string_total(item),
			suffixtree_fcs_item_score(item));
		free(buf);
	}
	sarray_each_type_end_m;
}

static void out_fcs_array_state(sarray_s *array, state_t state)
{
	fcs_item item;
	char *buf;
	printf("Fcs Array State:\n");
	sarray_each_type_m(item, fcs_item, array) {
		if (suffixtree_fcs_item_state(item) < state)
			continue;
		buf = s16_out_to_buff(suffixtree_fcs_item_string(item));
		if (buf == NULL) {
			printf("error string!!!\n");
			continue;
		}
		printf("fcs-> [state:0x%x][string:%s] [sidx:%d] [eidx:%d] [leafTotal:%d] [stringTotal:%d] [score:%f]\n",
			suffixtree_fcs_item_state(item),
			buf,
			suffixtree_fcs_item_startidx(item),
			suffixtree_fcs_item_endidx(item),
			suffixtree_fcs_item_leaf_total(item),
			suffixtree_fcs_item_string_total(item),
			suffixtree_fcs_item_score(item));
		free(buf);
	}
	sarray_each_type_end_m;
}

int readLine(FILE *fd, char* buff, int max)
{
	int i = 0;
	char* tem = buff;
	char c;

	while ( (*buff++ = c = fgetc(fd)) != EOF && *(buff - 1) != '\n' ) {
		i++;
		if (i > max)
			break;
	}
	tem[i] = '\0';
	if (c == EOF) i = -1;
	return i;
}

void usage()
{
	printf("fcs in_list out_list [-info]\n");
}

int main(int argc, char *argv[])
{
	suffixtree st;
	string16 *s16;
	sarray_s fcs_array;
	sarray_s fcs_str_array;
	char buff[1024];
	char *in;
	char *out;
	int info = 0;
	int len;
	FILE *in_fd, *out_fd;
	if (argc < 3) {
		usage();
		exit(0);
	}
	in = argv[1];
	out = argv[2];
	if (argc == 4 && !strcmp(argv[3], "-info"))
		info = 1;
	in_fd = fopen(in, "r");
	if (in_fd == NULL) {
		printf("open in file %s fail!!\n", in);
		exit(0);
	}
	out_fd = fopen(out, "w");
	if (out_fd == NULL) {
		printf("open out file %s fail!!\n", in);
		exit(0);
	}
	sarray_resource_init(&fcs_array);
	sarray_resource_init(&fcs_str_array);
	s16 = s16_new(100);
	st = suffixtree_create();
	suffixtree_use_node_cache(st, 1);
	suffixtree_set_encode(st, suffixtree_encode_gb2312);
	while (1) {
		len = readLine(in_fd, buff, 1024);
		if (len == 0) continue;
		if (len == -1) break;
		s16_setcstr(s16, buff);
		suffixtree_insert(st, s16);
	}
	suffixtree_statistic(st);
	suffixtree_fcs(st, 0.4, &fcs_array);
	suffixtree_fcs_string_array_state(&fcs_array, &fcs_str_array, FCS_STATE_FILTER);
	out_fcs_str_to_file(out_fd, &fcs_str_array);

	if (info == 1) {
		suffixtree_out_textmatrix(st);
		suffixtree_out_suffixtree_string(st);
		suffixtree_out_suffixtree(st);

		out_fcs_str(&fcs_str_array);
		out_fcs_array(&fcs_array);
		out_fcs_array_state(&fcs_array, FCS_STATE_FILTER);
	}
	suffixtree_fcs_clean(&fcs_array);
	suffixtree_clean(st);
	s16_free(s16);
	sarray_resource_clear(&fcs_array);
	sarray_resource_clear(&fcs_str_array);
	fclose(out_fd);
	fclose(in_fd);
	return 0;
}
