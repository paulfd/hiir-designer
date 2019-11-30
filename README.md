# hiir-designer
Designs oversampling stages adapter to HIIR from Laurent de Soras (http://ldesoras.free.fr/prod.html).

Build using your favorite compiler, e.g. for clang
```
clang++ hiir-designer.cpp hiir/PolyphaseIir2Designer.cpp -I. -o hiir-designer
```

Usage
```
Usage: ./hiir-designer n_stages transition_bw [stopband_attenuation]

n_stages is the number of 2x up/down sampling stages
transition_bw is the transition bandwidth of the first stage
stopband_attenuation is the expected stopband attenuation
```