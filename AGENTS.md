# Repository Guidelines

## Project Structure & Module Organization

This is a documentation-only repository for the ALINX AXU5EV/AXU5EVB-P Zynq MPSoC platforms. All deliverables currently live in the repository root as PDF files:

- `course_s0_...` through `course_s6_...`: ordered development courses (tool setup, FPGA, Vitis, Linux, drivers, applications, and HLS).
- `AXU5EV-P开发板用户手册.pdf` and `AXU5EVB-P开发板用户手册.pdf`: board-specific reference manuals.

There are no application sources, generated build artifacts, or automated tests. Keep new documentation at the root unless a clearly scoped subdirectory is introduced.

## Build, Test, and Development Commands

No build step is required. Before submitting a document, check that it is readable and structurally valid:

```bash
pdfinfo "AXU5EV-P开发板用户手册.pdf"
pdftotext -layout "AXU5EV-P开发板用户手册.pdf" /tmp/axu5ev-check.txt
```

Use `qpdf --check <file>.pdf` when available to detect damaged PDF structure. Review extracted text (and page rendering when layout matters) for missing pages, garbled Chinese text, broken links, or clipped figures.

## Coding Style & Naming Conventions

For PDFs, preserve the existing descriptive naming scheme: lowercase `course_sN_` prefixes for course order and the board name for manuals. Keep version identifiers in filenames (for example, `V1.03`) and use UTF-8 Chinese names where supplied by the source. Avoid spaces, duplicate copies, and opaque names. If Markdown or scripts are added later, use UTF-8, four-space indentation, and names that mirror the related document.

## Testing Guidelines

Testing is manual document QA. Confirm the file opens in a standard PDF viewer, page count matches the source, text search works for Chinese and English terms, and images/tables render correctly. Run the validation commands above for every changed PDF; record any intentional limitations in the pull request.

## Commit & Pull Request Guidelines

No Git history is present, so no repository-specific commit convention can be inferred. Use concise imperative subjects such as `Add AXU5EV Linux driver manual` or `Replace course_s2 PDF with V1.04`, and keep each commit focused. Pull requests should explain the document/version changed, identify replaced or added files, include validation results, and note licensing or source provenance. Add screenshots only when demonstrating a rendering fix.

## Security & Configuration Tips

Treat PDFs as untrusted binaries: do not enable embedded scripts or macros, and avoid committing credentials, private board logs, or unrelated build outputs. Prefer preserving the original file metadata and documenting the source when replacing a manual.
