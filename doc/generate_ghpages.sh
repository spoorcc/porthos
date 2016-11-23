#! /usr/bin/bash

gh_pages_branch_dir="doc/html"
pdf_file_name="Porthos.pdf"

current_dir=${PWD##*/}

build_project() {

    mkdir -p bld
    cd bld
    cmake ..
    make doc
    cd ${project_root}
}

commit_gh_pages() {

    cp -R bld/doc/html/* $gh_pages_branch_dir

    git add $gh_pages_branch_dir/*
    git commit -m "Updated docs"

}

push_remote() {
    cd ${project_root}
    git push origin `git subtree split --prefix $gh_pages_branch_dir master`:gh-pages --force
}

copy_pdf_to_doc_root() {

    cd ./bld/doc/latex
    make pdf
    cd -

    cp bld/doc/latex/refman.pdf "$gh_pages_branch_dir/$pdf_file_name"
}


if [ "${current_dir}" == "doc" ]; then
    cd ..
    current_dir=${PWD##*/}
fi

if [ "${current_dir}" == "porthos" ]; then

    project_root=${PWD}
    build_project
    copy_pdf_to_doc_root
    commit_gh_pages
    push_remote
else
    echo "Must be in project root"
fi
