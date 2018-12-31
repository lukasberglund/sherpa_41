#include "display.hpp"

#include "parser/css.hpp"
#include "parser/html.hpp"

#include <gtest/gtest.h>

class DisplayTest : public ::testing::Test {};

using namespace Display;

TEST_F(DisplayTest, CommandCtorDtor) {
    RectangleCmd rectangleCmd(Layout::Rectangle(0, 0, 0, 0),
                              CSS::ColorValue(0, 0, 0, 0));
}

TEST_F(DisplayTest, CreateQueueEmpty) {
    auto queue = Command::createQueue(
        Layout::BoxPtr(new Layout::AnonymousBox()));
    ASSERT_TRUE(queue.empty());
}

TEST_F(DisplayTest, CreateQueue) {
    HTMLParser html("<html><div></div></html>");
    CSSParser  css("* { background: #000000; display: block; padding: 12px; }");
    auto       style = Style::StyledNode::from(html.evaluate(), css.evaluate());
    auto       layout = Layout::Box::from(style,
                                    Layout::BoxDimensions(
                                        Layout::Rectangle(0, 0, 800, 600)));
    auto       queue  = Command::createQueue(layout);
    ASSERT_EQ(queue.size(), 10);  // 5 x html, 5 x div

    const auto & htmlTag = dynamic_cast<RectangleCmd *>(queue.front().get());
    ASSERT_FALSE(htmlTag == nullptr);

    // first command: html
    ASSERT_EQ(htmlTag->getColor().print(), "rgba(0, 0, 0, 255)");
    ASSERT_EQ(htmlTag->getRectangle().width, 800);
    ASSERT_EQ(htmlTag->getRectangle().height, 48);

    for (uint64_t i = 0; i < 5; ++i)
        queue.pop();  // html borders

    const auto & divTag = dynamic_cast<RectangleCmd *>(queue.front().get());

    // second command: div
    ASSERT_EQ(divTag->getColor().print(), "rgba(0, 0, 0, 255)");
    ASSERT_EQ(divTag->getRectangle().width, 776);
    ASSERT_EQ(divTag->getRectangle().height, 24);
}