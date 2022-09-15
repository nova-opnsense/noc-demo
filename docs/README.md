# Firmware Documents

1. This repo will include all documents for Firmware Team.
2. The document is written by using asciidoctor and plantuml.
3. The document can be generate to PDFs or document websites.

## Generate PDF

### MAC OS Setup

- Install java runtime: 
    ```
    https://java.com/en/download
    ```

- Install Home Brew
    ```
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```

- Install asciidoctor command:
    ```
    sudo gem install -n /usr/local/bin asciidoctor-pdf --pre

    ```

- Install asciidoctor diagram compponent:

    ```
    sudo gem install asciidoctor-diagram
    ```

- Install Graphviz:

    ```
    brew install graphviz
    ```

### Ubuntu Setup

- Install ruby: 
    ```
    sudo apt-get install ruby
    ```

- Install asciidoctor command:
    ```
    sudo gem install -n /usr/local/bin asciidoctor-pdf --pre

    ```

- Install asciidoctor diagram compponent:

    ```
    sudo gem install asciidoctor-diagram
    ```

- Install Graphviz:

    ```
    sudo get-apt install graphviz
    ```

### Generate Documents

- Generate document in PDF format command:
    ```
    ./adoc_generate_pdf.sh {DOCUMENT_FILE}
    ```

- Example command:
    ```
    ➜  sw-docs git:(main) ✗ ./adoc_generate_pdf.sh thing_service/hub/device_api.adoc
    Convert thing_service/hub/device_api.adoc to pdf
    Convert thing_service/hub/device_api.adoc to pdf success: thing_service/hub/device_api_20220225-1136.pdf
    ```

## Generate Antora

- Install antora:

    ```
    npm i -g @antora/cli@3.0.0 @antora/site-generator@3.0.0
    ```

- Install assciidoctor:

    ```
    npm i -g asciidoctor asciidoctor-plantuml
    ```

- Install http server:

    ```
    npm i -g http-server
    ```

- Build antora:

    ```
    antora playbook.yml
    ```

- Run document server:

    ```
    http-server build/site
    ```
    
## Authors:
- Khoi Nguyen
- Forked from Cong Nga Le - SW team leader