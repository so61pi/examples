package main

type XInterface interface {
	VeryLongFunctionName()
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

// Wrap size is 40 bytes
// = 8 (int) + 16 (interface{}) + 16 (XInterface)
type Wrap struct {
	x int
	y interface{}
	z XInterface
}

/*
struct GoInterface {
    ptrTypeInfo : pointer to type info
    ptrObject   : pointer to underlying object
}
*/

/*
;
; non-pointer version
;
__go_imt_I20_VeryLongFunctionNameFee__N9_main.Test:         ; This is basically a vtable with type info.
        .quad   __go_tdn_main.Test                          ;     pointer to type info (below)
        .quad   main.VeryLongFunctionName.N9_main.Test      ;     pointer to VeryLongFunctionName

__go_tdn_main.Test:
        .byte   -103
        .byte   8
        .byte   8
        .zero   5
        .quad   16
        .long   432309522
        .zero   4
        .quad   runtime.memhash128$descriptor
        .quad   runtime.memequal128$descriptor
        .quad   __go_td_S7_.main.aN3_int7_.main.bS7_.main.cN3_intee$gc
        .quad   C11
        .quad   C19
        .quad   __go_td_pN9_main.Test
        .quad   C41
        .quad   2
        .quad   2

;
; pointer version
;
__go_pimt__I20_VeryLongFunctionNameFee__N9_main.Test:
        .quad   __go_td_pN9_main.Test
        .quad   main.VeryLongFunctionName.N9_main.Test

__go_td_pN9_main.Test:
        .byte   54
        .byte   8
        .byte   8
        .zero   5
        .quad   8
        .long   -1672982231
        .zero   4
        .quad   runtime.memhash64$descriptor
        .quad   runtime.memequal64$descriptor
        .quad   __go_td_pN9_main.Test$gc
        .quad   C20
        .quad   C26
        .quad   __go_td_ppN9_main.Test
        .quad   __go_tdn_main.Test
*/

func main() {
	a := Test{}


	w1 := Wrap{}
	w1.x = 12345678
	w1.y = a    // ca = new copy of a
				// ptrTypeInfo = __go_tdn_main.Test
				// ptrObject = &ca
	w1.z = a    // ca = new copy of a
				// ptrTypeInfo = __go_imt_I20_VeryLongFunctionNameFee__N9_main.Test
				// ptrObject = &ca
	Call(&w1)


	w2 := Wrap{}
	w2.x = 87654321
	w2.y = &a   // ptrTypeInfo = __go_tdn_main.Test
				// ptrObject = &a
	w2.z = &a   // ptrTypeInfo = __go_pimt__I20_VeryLongFunctionNameFee__N9_main.Test
				// ptrObject = &a
	Call(&w2)

	v := w1.y   // v and w1.y point to the same underlying object,
				// no new copy of underlying object is created.
				// v.ptrTypeInfo = w1.y.ptrTypeInfo
				// v.ptrObject = w1.y.ptrObject
	CallEmpty(v)
}

func Call(w *Wrap) {
	a, ok1 := w.y.(XInterface)  // No new copy of underlying object is created.
	if ok1 {
		a.VeryLongFunctionName()
	}

	b, ok2 := w.y.(Test)        // New copy of Test is created.
	if ok2 {
		b.VeryLongFunctionName()
	}

	c, ok2 := w.y.(*Test)		// No new copy of underlying object is created.
	if ok2 {
		c.VeryLongFunctionName()
	}
}

func CallEmpty(v interface{}) {}
