package gomockdemo

type Ipeople interface {
    GetName() string
    SetName(string) string
}

func GetPeopleName(mi Ipeople) string {
    mi.GetName()
    return mi.GetName()
}

func SetPeopleName(mi Ipeople, name string) string {
     return  mi.SetName(name)
}
