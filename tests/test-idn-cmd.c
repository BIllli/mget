/*
 * Copyright(c) 2013 Tim Ruehsen
 *
 * This file is part of libmget.
 *
 * Libmget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Libmget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libmget.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Testing Mget
 *
 * Changelog
 * 17.07.2013  Tim Ruehsen  created
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h> // exit()
#include "libtest.h"

// Kon'nichiwa <dot> Japan
#define euc_jp_hostname "\272\243\306\374\244\317.\306\374\313\334"
#define utf8_hostname "\344\273\212\346\227\245\343\201\257.\346\227\245\346\234\254"
#define punycoded_hostname "xn--v9ju72g90p.xn--wgv71a"

int main(void)
{
	mget_test_url_t urls[]={
		{	.name = "http://" punycoded_hostname "/index.html",
			.code = "200 Dontcare",
			.body = "What ever",
			.headers = {
				"Content-Type: text/plain",
			}
		},
	};

	char options[256];

	// functions won't come back if an error occurs
	mget_test_start_http_server(
		MGET_TEST_RESPONSE_URLS, &urls, countof(urls),
		0);

	// test-idn-cmd
	snprintf(options, sizeof(options),
		"--iri -rH -e http_proxy=localhost:%d --local-encoding=EUC-JP " euc_jp_hostname,
		mget_test_get_server_port());

	mget_test(
//		MGET_TEST_KEEP_TMPFILES, 1,
		MGET_TEST_OPTIONS, options,
		MGET_TEST_REQUEST_URL, NULL,
		MGET_TEST_EXPECTED_ERROR_CODE, 0,
		MGET_TEST_EXPECTED_FILES, &(mget_test_file_t []) {
			{ punycoded_hostname "/index.html", urls[0].body },
			{	NULL } },
		0);

	// test-idn-cmd
	snprintf(options, sizeof(options),
		"--iri -rH -e http_proxy=localhost:%d --local-encoding=UTF-8 " utf8_hostname,
		mget_test_get_server_port());

	mget_test(
//		MGET_TEST_KEEP_TMPFILES, 1,
		MGET_TEST_OPTIONS, options,
		MGET_TEST_REQUEST_URL, NULL,
		MGET_TEST_EXPECTED_ERROR_CODE, 0,
		MGET_TEST_EXPECTED_FILES, &(mget_test_file_t []) {
			{ punycoded_hostname "/index.html", urls[0].body },
			{	NULL } },
		0);

	exit(0);
}
