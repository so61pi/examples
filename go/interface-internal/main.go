// Compiler:        gcc 7.2.0 x86
// Optimization:    -O0
// Assembly:        Intel
//
// https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/type.go
// https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/iface.go#L32
// https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/slice.go#L20
// https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/runtime2.go#L145
package main

type XInterface interface {
	VeryLongFunctionName()
	AnotherLongFunction()
}

// Test size is 16 bytes
type Test struct {
	a int
	b struct {
		c int
	}
}

// The code for copying a Test instance is placed right above the code of VeryLongFunctionName.
// So the function below becomes something like this
//     func (t *Test) VeryLongFunctionName() {
//         newt := *t
//         // Use newt now.
//     }
func (t Test) VeryLongFunctionName() {}

func (t Test) AnotherLongFunction() {}

// Wrap size is 40 bytes
// = 8 (int) + 16 (interface{}) + 16 (XInterface)
type Wrap struct {
	x int
	y interface{}
	z XInterface
}

/*
type iface struct {
    // tab points to type descriptor struct.
    // - For empty interface, it is one of:
    //   + interfacetype
    //   + maptype
    //   + arraytype
    //   + chantype
    //   + slicetype
    //   + functype
    //   + ptrtype
    //   + structtype
    // - For non-empty interface, it is itab.
    tab  unsafe.Pointer

    // data points to the underlying object of an interface instance.
    data unsafe.Pointer
}
*/

func main() {
	a := Test{}

	// ======================================== //
	w1 := Wrap{}
	w1.x = 12345678

	w1.y = a
	/* ca = new copy of a
	 * tab = __go_tdn_main.Test (which is interfacetype)
	 * data = &ca
	 */

	w1.z = a
	/* ca = new copy of a
	 * tab = __go_imt_I19_AnotherLongFunctionFe20_VeryLongFunctionNameFee__N9_main.Test (which is itab for value)
	 * data = &ca
	 */

	Call(&w1)

	// ======================================== //
	w2 := Wrap{}
	w2.x = 87654321

	w2.y = &a
	/* tab = __go_td_pN9_main.Test (which is ptrtype)
	 * data = &a
	 */

	w2.z = &a
	/* tab = __go_pimt__I19_AnotherLongFunctionFe20_VeryLongFunctionNameFee__N9_main.Test (which is itab for pointer)
	 * data = &a
	 */

	Call(&w2)

	// ======================================== //
	v := w1.y
	/* v and w1.y point to the same underlying object,
	 * no new copy of underlying object is created.
	 * v.tab = w1.y.tab
	 * v.data = w1.y.data
	 */
	CallEmpty(v)
}

func Call(w *Wrap) {
	// Call ifaceE2I2 to convert empty interface to non-empty interface.
	// ifaceE2I2 calls getitab to check whether the underlying type implements
	// requested interface, then build a new itab (this operation requires
	// memory allocation).
	a, ok1 := w.y.(XInterface) // No new copy of underlying object is created.
	if ok1 {
		a.VeryLongFunctionName()
	}

	b, ok2 := w.y.(Test) // New copy of Test is created.
	if ok2 {
		b.VeryLongFunctionName()
	}

	c, ok2 := w.y.(*Test) // No new copy of underlying object is created.
	if ok2 {
		c.VeryLongFunctionName()
	}
}

func CallEmpty(v interface{}) {}
