package main

import (
	"io"
	"log"
	"os"
	"regexp"
	"strconv"
	"strings"

	"github.com/elamre/vcd"
)

var rgx = regexp.MustCompile("^(.*): (\\d+) (\\d+)$")

type chg struct {
	time uint64
	wire string
	val  string
}

func DumpVcd(vals []chg) {
	if len(vals) == 0 {
		return
	}

	writer, err := vcd.New("/dev/stdout", "1ns")
	if err != nil {
		log.Fatal(err)
	}
	defer writer.Close()
	writer.SetVersion("1.0.0").SetComment("Logic analyzer VCD dump")

	varm := make(map[string]bool)
	for _, v := range vals {
		varm[v.wire] = true
	}
	vars := make([]vcd.VcdDataType, 0, len(varm))
	for k := range varm {
		vars = append(vars, vcd.NewVariable(k, "wire", 1))
	}
	writer.RegisterVariables("pins", vars...)

	for _, v := range vals {
		writer.SetValue(v.time, v.val, v.wire)
	}

	writer.SetTimestamp(vals[len(vals)-1].time + 100)
}

func ParseChgs(in string) []chg {
	lines := strings.Split(in, "\n")
	var chgs []chg
	for _, l := range lines {
		submatch := rgx.FindStringSubmatch(l)
		if submatch != nil && len(submatch) == 4 {
			time, err := strconv.ParseUint(submatch[2], 10, 64)
			if err != nil {
				log.Fatal(err)
			}
			chgs = append(chgs, chg{
				val:  submatch[3],
				wire: submatch[1],
				time: time,
			})
		}
	}
	return chgs
}

func main() {
	in, err := io.ReadAll(os.Stdin)
	if err != nil {
		log.Fatal(err)
	}
	DumpVcd(ParseChgs(string(in)))
}
