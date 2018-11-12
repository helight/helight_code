package main

import (
    "github.com/yuin/gopher-lua"
)


func Double(L *lua.LState) int {
    lv := L.ToInt(1)             /* get argument */
    L.Push(lua.LNumber(lv * 2 + 23)) /* push result */
    return 1                     /* number of results */
}

func main () {
	L := lua.NewState()
	defer L.Close()
	if err := L.DoString(`print("hello")`); err != nil {
		panic(err)
	}

	L.SetGlobal("double", L.NewFunction(Double)) /* Original lua_setglobal uses stack... */

	if err := L.DoString(`print(double(3000))`); err != nil {
		panic(err)
	}
}