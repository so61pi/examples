==================================
 reStructuredText Simple Template
==================================

.. contents:: Table of Contents
.. section-numbering::

Chapter 1
=========

Normal text.

*Italic text.*

**Bold text.**

``inline code``.

    Quoted text starts with 4-space indentation.

    --- Anonymous.

Subscript :sub:`subscript`.

Superscript :sup:`superscript`.

Code blocks should use 4-space indentation:

.. code-block:: c

    int main(int argc, char* argv[]) {
        return 0;
    }

Indented code block starts with 2-space:

  .. code-block:: c

      int main(int argc, char* argv[]) {
          return 0;
      }

Literal block starts with double-colon at the end of the preceding pararaph::

    int main(int argc, char* argv[]) {
        return 0;
    }

::

    Preceding paragraph doesn't have to have any text, but must end with double-colon to start a literal block.

Separator ahead!!!

-----

- A bullet list

  Paragraph.

  * Nested bullet list must use 2-space indentation.
  * Nested item 2.

    + Third level.
    + Item 2 with same level code block.

      .. code-block:: c

          int main(int argc, char* argv[] {
              return 0;
          }

    + Item 3 with indented code block.

        .. code-block:: c

            int main(int argc, char* argv[] {
                return 0;
            }

    + Item 4.

  * Nested item 3.

#. Auto-enumerated 1.

#. Auto-enumerated 2.

5. List can start at arbitrary number.

#. Subsequent auto-enumerated items still work correctly.

=====  =====  ======
   Inputs     Output
------------  ------
  A      B    A or B
=====  =====  ======
False  False  False
True   False  True
False  True   True
True   True   True
=====  =====  ======

+------------------------+------------+----------+----------+
| Header row, column 1   | Header 2   | Header 3 | Header 4 |
| (header rows optional) |            |          |          |
+========================+============+==========+==========+
| body row 1, column 1   | column 2   | column 3 | column 4 |
+------------------------+------------+----------+----------+
| body row 2             | Cells may span columns.          |
+------------------------+------------+---------------------+
| body row 3             | Cells may  | - Table cells       |
+------------------------+ span rows. | - contain           |
| body row 4             |            | - body elements.    |
+------------------------+------------+----------+----------+
| body row 5             | Cells may also be     |          |
|                        | empty: ``-->``        |          |
+------------------------+-----------------------+----------+

See `References`_ for more complex structures.

Section 1 [`ref <http://docutils.sourceforge.net/docs/user/rst/demo.html>`__]
-----------------------------------------------------------------------------

Sub-section 1
~~~~~~~~~~~~~

Sub-sub-section 1
`````````````````

Sub-sub-sub-section 1
'''''''''''''''''''''

Sub-sub-sub-section 2
'''''''''''''''''''''

Sub-sub-section 2
`````````````````

Sub-section 2
~~~~~~~~~~~~~

Section 2
---------

References
==========

- http://docutils.sourceforge.net/docs/user/rst/demo.html and `source <http://docutils.sourceforge.net/docs/user/rst/demo.txt>`__
- `Quickstart <http://docutils.sourceforge.net/docs/user/rst/quickstart.html>`__
