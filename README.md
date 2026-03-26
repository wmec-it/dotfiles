<div align="center">
    <picture align="center">
        <img src=".github/assets/wmec.png" alt="West-Mec Logo" width="400" />
    </picture>
</div>

<section id="code-syntax">
  <h2>Code Syntax</h2>
  
  <details open>
    <summary><h3 style="display:inline">Naming Convention</h3></summary>
    
    <div style="margin-top: 1rem;">
      <p><b>Format:</b></p>
      <pre><code>[_]WMECDOTFILES_&lt;DECLARATION_TYPE&gt;_&lt;LAYER_NAME&gt;[_]</code></pre>
      
      <h4>Components:</h4>
      <ul>
        <li>
          <code>&lt;IFIS_INTERNAL_INDICATOR&gt;</code>
          <p style="margin: 0.5rem 0; color: #8b949e;">Specifies the declaration's <i>scope</i>.</p>
          <ul>
            <li>
              <code>EXAMPLE_VARIABLE</code> — Global variables that run at every shell context
              <p style="margin: 0.5rem 0; color: #8b949e; font-size: 0.9rem;">Declared when <code>.bashrc</code> is sourced (from env, profile, *rc, etc)</p>
            </li>
          </ul>
        </li>
        <li>
          <code>&lt;DECLARATION_TYPE&gt;</code>
          <p style="margin: 0.5rem 0; color: #8b949e;">The type of declaration (function, alias, variable, etc.)</p>
        </li>
        <li>
          <code>&lt;LAYER_NAME&gt;</code>
          <p style="margin: 0.5rem 0; color: #8b949e;">Hierarchical organization (shell, editor, system, etc.)</p>
        </li>
      </ul>
    </div>
  </details>
  
</section>
