#include "parser_html.hpp"
#include "printer.hpp"

#include "./util.hpp"

#include <gtest/gtest.h>

// The fixture for testing the vector class
class HTMLParserTest : public ::testing::Test {};

TEST_F(HTMLParserTest, RootHTMLTag) {
    HTMLParser parser("<html></html>");
    ASSERT_PRINT(parser.evaluate(), R"(
<html>
</html>
)");
}

TEST_F(HTMLParserTest, NoRootHTMLTag) {
    HTMLParser parser("");
    ASSERT_PRINT(parser.evaluate(), R"(
<html>
</html>
)");
}

TEST_F(HTMLParserTest, Text) {
    HTMLParser parser("Hello, world!");
    ASSERT_PRINT(parser.evaluate(), R"(
<html>
	Hello, world!
</html>
)");
}

TEST_F(HTMLParserTest, Comment) {
    HTMLParser parser("<!--  \t\t\nThis is a <span>\n     comment      -->");
    ASSERT_PRINT(parser.evaluate(), R"(
<html>
	<!-- This is a <span>
     comment -->
</html>
)");
}

TEST_F(HTMLParserTest, Attributes) {
    auto html = R"(
<html lang="en" itemtype="schema">
</html>
)";

    ASSERT_PRINT(HTMLParser(html).evaluate(), html);
}

TEST_F(HTMLParserTest, NestedElements) {
    auto html = R"(
<html lang="en" itemtype="schema">
	<head>
		<!-- Titles are mandatory by the HTML spec -->
		<title>
			My Website
		</title>
	</head>
	<body>
		<main class="full-width">
			<p>
				First paragraph
				<a id="homepage" href="http://0.0.0.0:3000">
					<!-- change later -->
					Local Link
				</a>
			</p>
		</main>
	</body>
</html>
)";

    ASSERT_PRINT(HTMLParser(html).evaluate(), html);
}

TEST_F(HTMLParserTest, TagWhitespace) {
    HTMLParser parser("<  html       >   </ html     >");
    ASSERT_PRINT(parser.evaluate(), R"(
<html>
</html>
)");
}

TEST_F(HTMLParserTest, TextWhitespace) {
    HTMLParser parser("<html>  \n  Hel     lo    \t   </html>");
    ASSERT_PRINT(parser.evaluate(), R"(
<html>
	Hel     lo
</html>
)");
}