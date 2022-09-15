#!/usr/bin/env bash

function generate_pdf() {
    adoc_file=$1
    adoc_filename=${adoc_file%%.*}
    curdate=$(date +"%Y%m%d-%H%M")
    output_pdf=${adoc_filename}_${curdate}.pdf
    asciidoctor-pdf -r asciidoctor-diagram ${adoc_file} -o ${output_pdf}
    if [[ $? -eq 0 ]]
    then
        echo "Convert $adoc_file to pdf success: $output_pdf"
    else
        echo "have error, please check your file"
    fi
}
echo "Convert $@ to pdf"
generate_pdf $@