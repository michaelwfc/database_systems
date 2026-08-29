import pymupdf4llm
from .pymupdf_for_pdf import pdf_to_markdown
from pathlib import Path



def convert_pdf_to_markdown(
    input_pdf: str,
    out_path :str | None = None, tool="pymupdf4llm"
):
    """
    Convert PDF file to Markdown file.

    This is the public API used by cli.py.
    """
    if tool =="pymupdf4llm":
        md = pymupdf4llm.to_markdown(input_pdf)
        # print(md)
    elif tool=="pymupdf":
        md = pdf_to_markdown(input_pdf)

    if out_path is None:
        out_path = str(Path(input_pdf).with_suffix(".md"))
    Path(out_path).write_text(
        md,
        encoding="utf-8"
    )
    return md
    
def main():
    import argparse

    parser = argparse.ArgumentParser(description="Convert PDF to Markdown")
    parser.add_argument("input_pdf", help="Path to the input PDF file")
    # Make the output_md argument optional with a default value of None
    parser.add_argument("output_md", help="Path to the output Markdown file", nargs='?', default=None)
    args = parser.parse_args()

    convert_pdf_to_markdown(args.input_pdf, args.output_md)