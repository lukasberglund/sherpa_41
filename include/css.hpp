#ifndef CSS_HPP
#define CSS_HPP

#include "parser.hpp"

#include <numeric>
#include <set>
#include <unordered_set>
#include <vector>

class Visitor;

namespace CSS {

// forward declaration
struct Value;
struct Selector;
struct Declaration;
struct specificityOrder;

typedef std::unique_ptr<Value>                    ValuePtr;
typedef std::vector<uint64_t>                     Specificity;
typedef std::multiset<Selector, specificityOrder> PrioritySelectorSet;
typedef std::vector<Declaration>                  DeclarationSet;

/**
 * Abstract base struct representing a CSS declaration value
 */
struct Value {
   public:
    virtual ~Value() = default;

    /**
     * Dynamically clones a Value into a unique_ptr
     * @return cloned Value as a pointer
     */
    virtual ValuePtr clone() const = 0;

    /**
     * Prints a declaration value
     * @return printed value
     */
    virtual std::string print() const = 0;
};

/**
 * A text declaration value
 */
struct TextValue : public Value {
   public:
    ~TextValue() override = default;

    /**
     * Creates a text value
     * @param value text value
     */
    explicit TextValue(std::string value);

    TextValue(const TextValue & rhs) = default;

    /**
     * Clones a TextValue
     * @return cloned value
     */
    ValuePtr clone() const override;

    /**
     * Prints a text declaration value
     * @return printed value
     */
    std::string print() const override;

    std::string value;
};

enum Unit { px, em, rem, vw, vh };
std::vector<std::string> UnitRaw();
/**
 * A unit declaration value
 */
struct UnitValue : public Value {
   public:
    ~UnitValue() override = default;

    /**
     * Creates a unit value
     * @param value magnitude
     * @param unit unit used
     */
    UnitValue(double value, Unit unit);

    UnitValue(const UnitValue & rhs) = default;

    /**
     * Clones a UnitValue
     * @return cloned value
     */
    ValuePtr clone() const override;

    /**
     * Prints a unit declaration value
     * @return printed value
     */
    std::string print() const override;

    double value;
    Unit   unit;
};

/**
 * A color declaration value
 */
struct ColorValue : public Value {
   public:
    ~ColorValue() override = default;

    /**
     * Creates a color value
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    ColorValue(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    ColorValue(const ColorValue & rhs) = default;

    /**
     * Clones a ColorValue
     * @return cloned value
     */
    ValuePtr clone() const override;

    /**
     * Prints a color declaration value
     * @return printed value
     */
    std::string print() const override;

    uint8_t r, g, b, a;
};

/**
 * Represents a CSS selector. Can be a
 * - tag (body, a, p, span)
 * - id (#intro, #user-selection)
 * - class (.full-width, .click-toggle)
 * or any combination of those, but has at most one tag and one id.
 */
struct Selector {
   public:
    /**
     * Creates a CSS Selector
     * @param tag selector tag
     * @param id selector id
     * @param klass selector classes
     */
    explicit Selector(std::string              tag   = "",
                      std::string              id    = "",
                      std::vector<std::string> klass = {});

    /**
     * Determines the specificity of the selector, prioritized by
     * (id, class, tag). High specificity is more important.
     * @return specificity vector
     */
    Specificity specificity() const;

    /**
     * Prints a selector in the form `tag#id.class1.class2`
     * @return pretty-printed selector
     */
    std::string print() const;

    std::string              tag;
    std::string              id;
    std::vector<std::string> klass;
};

/**
 * Orders Selectors by decreasing specificity
 */
struct specificityOrder {
    bool operator()(const Selector & a, const Selector & b) const {
        return a.specificity() > b.specificity();
    }
};

/**
 * Represents a CSS declaration, of form `name: value;`
 */
struct Declaration {
   public:
    /**
     * Creates a CSS declaration
     * @param name declaration name
     * @param value declaration value
     */
    Declaration(std::string name, ValuePtr && value);

    /**
     * Copy ctor
     * @param rhs declaration to copy
     */
    Declaration(const Declaration & rhs);

    /**
     * Prints a declaration in the form `name: value;`
     * @return pretty-printed declaration
     */
    std::string print() const;

    std::string name;
    ValuePtr    value;
};

/**
 * Represents a CSS rule, of form
 * ```
 * .selector {
 *     some-name: arbitraryValue;
 * }
 * ```
 */
struct Rule {
   public:
    /**
     * Creates a CSS rule
     * @param selectors selectors to apply rule for
     * @param declarations declarations of rule
     */
    Rule(PrioritySelectorSet selectors, DeclarationSet declarations);

    PrioritySelectorSet selectors;
    DeclarationSet      declarations;
};

/**
 * A style sheet, consisting of CSS rules. Adapts a vector of rules to allow
 * for visitors.
 */
class StyleSheet : public std::vector<Rule> {
   public:
    /**
     * Accepts a visitor to the style sheet
     * @param visitor accepted visitor
     */
    void acceptVisitor(Visitor & visitor) const;
};
}  // namespace CSS

#endif