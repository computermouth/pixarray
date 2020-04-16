package main

import (
	"github.com/go-yaml/yaml"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"
	"text/template"
	"github.com/go-playground/colors"
)

type Layer_t struct {
	Layer     string `yaml:"layer"`
	Path      string `yaml:"path,omitempty"`
	XImage    int    `yaml:"x_image,omitempty"`
	YImage    int    `yaml:"y_image,omitempty"`
	Color     string `yaml:"color"`
	TrueColor [3]uint8
	X         []int `yaml:"x,omitempty"`
	Y         []int `yaml:"y,omitempty"`
}

type Sprite_t struct {
	Spritename string
	Animations []struct {
		Animation string `yaml:"animation"`
		Frames    []struct {
			Frame  string `yaml:"frame"`
			Delay  int    `yaml:"delay"`
			Layers []Layer_t `yaml:"layers"`
		} `yaml:"frames"`
	} `yaml:"animations"`
}

func main() {

    funcMap := template.FuncMap{
        "dec": func(i int) int {
            return i - 1
        },
        "truelayers": func(layers []Layer_t) int {
			truelayers := 0
			for i := 0; i < len(layers); i++ {
				if layers[i].Color != ""{
					truelayers++
				}
			}
            return truelayers
        },
        "toupper": func(s string) string {
            return strings.ToUpper(s)
        },
    }

	for i := 1; i < len(os.Args); i++ {

		fullpath := os.Args[i]
		directory := filepath.Dir(fullpath)
		filename := filepath.Base(os.Args[i])

		s := Sprite_t{Spritename: strings.TrimSuffix(filename, filepath.Ext(filename))}

		data, err := ioutil.ReadFile(fullpath)
		if err != nil {
			log.Fatalf("error: %v", err)
		}

		err = yaml.Unmarshal([]byte(data), &s)
		if err != nil {
			log.Fatalf("error: %v", err)
		}
		
		// format color codes
		for a, anim := range s.Animations {
			
			for f, frame := range anim.Frames{
				
				for l, layer := range frame.Layers{
					
					if layer.Color == "" {
						continue
					}
					
					hex, err := colors.Parse(layer.Color)
					if err != nil {
						log.Fatalf("error: %v -- %v", err, layer.Color)
					}
					
					s.Animations[a].Frames[f].Layers[l].TrueColor[0] = hex.ToRGB().R
					s.Animations[a].Frames[f].Layers[l].TrueColor[1] = hex.ToRGB().G
					s.Animations[a].Frames[f].Layers[l].TrueColor[2] = hex.ToRGB().B
				}
				
			}
			
		}
		
		// header
		cout := directory + "/" + s.Spritename + ".h"
		log.Printf(directory)
		log.Printf(cout)
		
		f, err := os.Create(cout)
		if err != nil {
			log.Fatalf("error: %v", err)
		}

		t := template.Must(template.New(filename).Funcs(funcMap).Parse(HTemplate))
		err = t.Execute(f, s)
		if err != nil {
			log.Fatalf("error: %v", err)
		}
		
		// c
		cout = directory + "/" + s.Spritename + ".c"
		log.Printf(directory)
		log.Printf(cout)
		
		f, err = os.Create(cout)
		if err != nil {
			log.Fatalf("error: %v", err)
		}

		t = template.Must(template.New(filename).Funcs(funcMap).Parse(CTemplate))
		err = t.Execute(f, s)
		if err != nil {
			log.Fatalf("error: %v", err)
		}
		
		f.Close()
	}

}
