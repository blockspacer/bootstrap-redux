// ----------------------------------------------------------------------------
// ____                               _
// |  _\                             | |
// | |_)| __ _ ___  ___  ___ ___   __| | ___ TM
// |  _< / _` / __|/ _ \/ __/ _ \ / _` |/ _ \
// | |_)| (_| \__ \  __/ (_| (_) | (_| |  __/
// |____/\__,_|___/\___|\___\___/ \__,_|\___|
//
//       C O M P I L E R  P R O J E C T
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// This software source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
//
// ----------------------------------------------------------------------------

#pragma once

namespace basecode::compiler::graphviz {

    enum class component_type_t {
        edge,
        node,
        graph,
        subgraph,
        cluster_subgraph
    };

    enum class attribute_type_t {
        background,
        arrowhead,
        arrowsize,
        arrowtail,
        bgcolor,
        center,
        charset,
        clusterrank,
        color,
        colorscheme,
        comment,
        compound,
        concentrate,
        constraint,
        decorate,
        dir,
        distortion,
        esep,
        fillcolor,
        fixedsize,
        fontcolor,
        fontname,
        fontpath,
        fontsize,
        forcelabels,
        gradientangle,
        group,
        headclip,
        headlabel,
        headport,
        height,
        image,
        imagepath,
        imagepos,
        imagescale,
        label,
        labelangle,
        labeldistance,
        labelfloat,
        labelfontcolor,
        labelfontname,
        labelfontsize,
        labeljust,
        labelloc,
        landscape,
        layer,
        layerlistsep,
        layers,
        layerselect,
        layersep,
        layout,
        lhead,
        ltail,
        margin,
        mclimit,
        minlen,
        newrank,
        nodesep,
        nojustify,
        normalize,
        nslimit,
        nslimit1,
        ordering,
        orientation,
        outputorder,
        overlap,
        pack,
        packmode,
        pad,
        page,
        pagedir,
        pencolor,
        penwidth,
        peripheries,
        pos,
        quantum,
        rank,
        rankdir,
        ranksep,
        ratio,
        regular,
        remincross,
        rotate,
        samehead,
        sametail,
        samplepoints,
        scale,
        searchsize,
        sep,
        shape,
        shapefile,
        showboxes,
        sides,
        size,
        skew,
        sortv,
        splines,
        style,
        tailclip,
        taillabel,
        tailport,
        viewport,
        voro_margin,
        weight,
        width,
        xlabel,
        z
    };

    enum class attribute_value_type_t {
        color,
        string,
        boolean,
        integer,
        color_list,
        floating_point,
    };

}