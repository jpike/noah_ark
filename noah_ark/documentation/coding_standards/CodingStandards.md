This file documents coding standards used for the Noah Ark project.  The goal of these standards is to ensure high quality code that makes all team members effective programmers.

Thoughts and suggestions are welcome.  This document may be updated in the future.

## Comments ##

Quality comments (using proper grammar) are heavily encouraged.  All classes (or other types), functions, and member variables should be documented using [Doxygen-style comments](http://www.stack.nl/~dimitri/doxygen/) using @ rather than \.  Brief/detailed descriptions, parameters, return values, and exceptions should all be documented.

The following types of comments are encouraged inside function bodies (although they may be applicable to other types of documentation):

* Intent comments - These are the most valuable type of comments.  They communicate things that may not be readily available from simply reading the code.  Providing intent helps future programmers gain insight into the mind of the original programmer and often answers the question "Why?"
* Tricky code comments - Any type of "trick" used in the code should be documented.  If it takes more than a few seconds to understand a line or two of code, the code is probably too tricky.  It is always preferable to try and rewrite code to remove tricks so that it is simple and more self-documenting, but comments help when this is not possible.
* Summary comments - Comments that summarize a block of code are less valuable but still encouraged.  Primarily, summary comments (not using the summary XML tags) inside methods help improve scannability of the code.  Readers can quickly read one-line summary comments to skip over or find sections of code of interest.

### Author / Date Documentation ###

Doxygen style @author and @date tags are optional.  Version control systems can provide insight into authors and dates of specific code modifications.  However, sometimes it is quicker to easily see the author directly in the code, which could speed up figuring out who to contact if you have a question about a piece of code.  @author tags can also be a way to properly credit authors.

If you choose to add @author and @date tags to class or method documentation, they should go after all other Doxygen-style tags.  For the author, you may choose to use your real name or an online screen name.

The @author of a class should indicate the primary person responsible for the current state of the class, so it is not necessary to update the @author of a class unless you make significant modifications.  If you are simply adding/modifying a single method, it would be more appropriate to have a separate @author for the new/modified method.  Remember, the purpose of any @author tag would be to help others find who to ask if they have questions.


## Naming Conventions ##

Naming follows the conventions described below.  These conventions are largely arbitrary based on slight preferences that have been gained (and changed) over time.

* **Classes/Structs (and other data types, like Enums/Typedefs)** - Use UpperCamelCase.
* **Interface Classes** - Use UpperCamelCase, with the name of the interface prefixed with an 'I' (ex. IAnInterface).
* **Member Variables** - Use UpperCamelCase.  Previously, member variables used to be prefixed with 'm_', but this has been deemed to provide little value and hurts readability in some instances.  You may see older code using the 'm_' prefix.  When updating this older code, you may remove 'm_' but are not required to.  However, the preference is to maintain consistency with the code you are updating, so either update all member variables to remove the 'm_' prefix or keep all member variables using the 'm_' prefix.
* **Local Variables/Parameters** - Use snake_case (an underscore between each word).  Previously, these variables used lowerCamelCase, but a preference has grown to using snake_case instead.  You may see older code using lowerCamelCase.  When updating this older code, you may change the casing to use snake_case but are not required to.  However, the preference is to maintain consistency with the code you are updating, so at least update the entirety of variables for the function/method being updated or keep all variables using lowerCamelCase.
* **Constants/Enum Values** - Use full capital letters with each word separated with an underscore (ex. THIS_IS_A_CONSTANT).
* **Functions/Methods** - Use UpperCamelCase.
* **Namespaces** - Use full capital letters with each word separated with an underscore (ex. THIS_IS_A_NAMESPACE).

## Curly Brace Style ##

All curly braces should go on separate lines.  This is simply a personal style choice to ensure consistency across the code base.

So this is acceptable:

```
void MyFunction()
{
}
```

But this is not:

```
void MyFunction() {
}
```


## Literals ##

Don't use literal numbers in calculations other than 0 or 1.  Create a well-named constant to hold the value and use the constant instead.

For example, don't do this:

```
score += 100;
```

Instead, do something like this:

```
score += POINTS_PER_ENEMY;
```

A similar thing applies to string literals.  If a string is used only in a single place, using a string literal may be okay, but as soon as the string needs to be used in more than one place, create a constant for it.


## Boolean Conditions ##

Boolean expressions, like those used in if statements or while loops, can become really tricky.  You should almost always use a single, well-named local variable or a boolean function to express a condition, rather than embedding calculations for conditions inside the if or while.

For example, don't do this:

```
if (position > 100)
```

Instead, do something like this (using the constant guideline for literals above):

```
bool exceeded_right_boundary = (position > RIGHT_BOUNDARY_X_POSITION);
if (exceeded_right_boundary)
```

Using well-named local variables (or boolean functions) can more clearly communicate the intended condition rather than leaving others to guess what each comparison means.  This is particularly important when boolean conditions are composed of multiple sub-conditions, so don't do stuff like this:

```
if (position > 100 && score > 500 || bonus == 2000 && state != "DEAD")
```

That condition is nearly impossible to understand.  Instead, you should create at least one local boolean variable to hold that condition (it may even be appropriate to create several and then combine them for the final condition).  Finally, don't be afraid to use parentheses around parts of a boolean expression - they can clarify things for other programmers and help prevent bugs.
