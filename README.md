# PDF2ePUB converter with layout analysis 

## Dependencies

- libqt4-dev
- libpoppler-qt4-dev
- libopencv-dev

## Compile
```
qmake
make
```

## Usage
WARNING: This is just a proof of concept sketch, there are many bugs.

- Run smaco01 executable
- Enter path to PDF file
- Click load
- Do cut or change page (cut is done after page changing)
- Set tresholds for continous blank cols or rows which separate two blocks
- Select block to extract text from

## Roadmap

- Guess block type (text, graphics, table, math, code)
- Semantic analysis and supervised semi-automated layout learning
- Generating ePUB output
