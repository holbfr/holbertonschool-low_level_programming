#include <stdlib.h>
#include <string.h>
#include "session.h"

session_t *session_create(const char *id, unsigned int uid, const unsigned char *data, size_t data_len)
{
	session_t *s;

	s = (session_t *)malloc(sizeof(*s));
	if (!s)
		return NULL;

	/*s->id = (char *)id;*/

	s->id = (char *)malloc((strlen(id) + 1) * sizeof(char));
	if (!s->id)
		return NULL;
	memcpy(s->id, id, strlen(id) + 1);
	
	s->uid = uid;

	if (data_len > 0) {
		s->data = (unsigned char *)malloc(data_len);
		if (!s->data) {
			return NULL;
		}
		memcpy(s->data, data, data_len);
		s->data_len = data_len;
	} else {
		s->data = NULL;
		s->data_len = 0;
	}

	return s;
}

int session_set_data(session_t *s, const unsigned char *data, size_t data_len)
{
	unsigned char *tmp;

	if (!s)
		return 0;

	if (data_len == 0) {
		free(s->data);
		s->data = NULL;
		s->data_len = 0;
		return 1;
	}

	if (!data)
		return 0;

	tmp = (unsigned char *)realloc(s->data, data_len);
	if (!tmp)
		return 0;
	
	s->data = tmp;

	memcpy(s->data, data, data_len);
	s->data_len = data_len;
	return 1;
}

void session_destroy(session_t *s)
{
	if (!s)
		return;

	free(s->id);

	free(s->data);
	free(s);
}
