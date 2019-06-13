// Compiler:        gcc 7.2.0 x86
// Optimization:    -O0
// Assembly:        Intel
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
struct GoInterface {
    // ptrTypeDesc points to type descriptor struct.
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
    ptrTypeDesc

    // ptrObject points to the underlying object of an interface instance.
    ptrObject
}

https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/type.go
https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/iface.go#L32
https://github.com/gcc-mirror/gcc/blob/gcc-7_2_0-release/libgo/go/runtime/slice.go#L20
*/

/*
;
; value to interface version
;
__go_imt_I19_AnotherLongFunctionFe20_VeryLongFunctionNameFee__N9_main.Test:     ; itab = pointer to type info + array of methods
        .quad   __go_tdn_main.Test
        .quad   main.AnotherLongFunction.N9_main.Test
        .quad   main.VeryLongFunctionName.N9_main.Test

__go_tdn_main.Test:                                                         ; interfacetype
        .byte   -103                                                        ; +- _type      +- kind       uint8
        .byte   8                                                           ; |             |  align      int8
        .byte   8                                                           ; |             |  fieldAlign uint8
        .zero   5                                                           ; |             |  _          uint8        (+padding)
        .quad   16                                                          ; |             |  size       uintptr
        .long   432309522                                                   ; |             |  hash       uint32
        .zero   4                                                           ; |             |                          (+padding)
        .quad   runtime.memhash128$descriptor                               ; |             |  hashfn  func(unsafe.Pointer, uintptr) uintptr
        .quad   runtime.memequal128$descriptor                              ; |             |  equalfn func(unsafe.Pointer, unsafe.Pointer) bool
        .quad   __go_td_S7_.main.aN3_int7_.main.bS7_.main.cN3_intee$gc      ; |             |  gc     unsafe.Pointer
        .quad   C12                                                         ; |             |  string *string
        .quad   C21                                                         ; |             |  *uncommontype
        .quad   __go_td_pN9_main.Test                                       ; |             +- ptrToThis *_type
        .quad   C44                                                         ; +- []imethod  +- array unsafe.Pointer
        .quad   2                                                           ;               |  len   int
        .quad   2                                                           ;               +- cap   int

;
; pointer to interface version
;
__go_pimt__I19_AnotherLongFunctionFe20_VeryLongFunctionNameFee__N9_main.Test:   ; itab = pointer to type info + array of methods
        .quad   __go_td_pN9_main.Test
        .quad   main.AnotherLongFunction.N9_main.Test
        .quad   main.VeryLongFunctionName.N9_main.Test

__go_td_pN9_main.Test:                          ; ptrtype
        .byte   54                              ; +- _type  +- kind       uint8
        .byte   8                               ; |         |  align      int8
        .byte   8                               ; |         |  fieldAlign uint8
        .zero   5                               ; |         |  _          uint8        (+padding)
        .quad   8                               ; |         |  size       uintptr
        .long   -1672982231                     ; |         |  hash       uint32
        .zero   4                               ; |         |                          (+padding)
        .quad   runtime.memhash64$descriptor    ; |         |  hashfn  func(unsafe.Pointer, uintptr) uintptr
        .quad   runtime.memequal64$descriptor   ; |         |  equalfn func(unsafe.Pointer, unsafe.Pointer) bool
        .quad   __go_td_pN9_main.Test$gc        ; |         |  gc     unsafe.Pointer
        .quad   C22                             ; |         |  string *string
        .quad   C29                             ; |         |  *uncommontype
        .quad   __go_td_ppN9_main.Test          ; |         +- ptrToThis *_type
        .quad   __go_tdn_main.Test              ; +- elem *_type

                        +-------------------------------------+
                        |                                     |
                        v                                     |
                   +----+----+         +-----------------+    |
                   | ptrtype |    +--->+  interfacetype  |    |
                   |         |    |    |                 |    |
                   | +-----+ |    |    | +-------------+ |    |
                   | |_type| |    |    | |    _type    | |    |
                   | +-----+ |    |    | | +---------+ | |    |
                   | |elem +------+    | | |ptrToThis+--------+
                   | +-----+ |         | | +---------+ | |
                   +---------+         | +-------------+ |
                        ^              +-----------------+
                        |                             ^
                        |                             |
+------------------+    |       +----------------+    |
| itab for pointer |    |       | itab for value |    |
|                  |    |       |                |    |
|    +-------+     |    |       |   +-------+    |    |
|    |type   +----------+       |   |type   +---------+
|    +-------+     |            |   +-------+    |
|    |methods|     |            |   |methods|    |
|    +-------+     |            |   +-------+    |
+------------------+            +----------------+
*/

func main() {
	a := Test{}

	// ======================================== //
	w1 := Wrap{}
	w1.x = 12345678

	w1.y = a
	/* ca = new copy of a
	 * ptrTypeDesc = __go_tdn_main.Test (which is interfacetype)
	 * ptrObject = &ca
	 */

	w1.z = a
	/* ca = new copy of a
	 * ptrTypeDesc = __go_imt_I19_AnotherLongFunctionFe20_VeryLongFunctionNameFee__N9_main.Test (which is itab for value)
	 * ptrObject = &ca
	 */

	Call(&w1)

	// ======================================== //
	w2 := Wrap{}
	w2.x = 87654321

	w2.y = &a
	/* ptrTypeDesc = __go_tdn_main.Test (which is ptrtype)
	 * ptrObject = &a
	 */

	w2.z = &a
	/* ptrTypeDesc = __go_pimt__I19_AnotherLongFunctionFe20_VeryLongFunctionNameFee__N9_main.Test (which is itab for pointer)
	 * ptrObject = &a
	 */

	Call(&w2)

	// ======================================== //
	v := w1.y
	/* v and w1.y point to the same underlying object,
	 * no new copy of underlying object is created.
	 * v.ptrTypeDesc = w1.y.ptrTypeDesc
	 * v.ptrObject = w1.y.ptrObject
	 */
	CallEmpty(v)
}

func Call(w *Wrap) {
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
